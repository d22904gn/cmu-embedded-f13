/** @file int_asm.S
 *
 * @brief Assembly assistance functions to handle interrupts.
 *
 * We handle interrupts in SVC mode with interrupts disabled.  Hence we perform
 * this elaborate waltz to transplant ourselves onto the svc stack, regardless
 * of the source of the IRQ.
 *
 * @author Kartik Subramanian
 * @date 2008-11-21
 */

/*
 * General idea behind this IRQ handler: allow for easier context
 * switches by only having to deal with one stack (SVC) instead of two
 * (SVC and IRQ). By giving each task its own kernel stack, if we switch
 * svc_sp then everything gets restored auto-magically.
 */

#include <asm.h>
#include <arm/psr.h>

    .file "irq_handler.S"

/*
 * Code to take an IRQ.
 */
FUNC(irq_handler)
	/* lr starts off pointing at next instruction + 4 -- fix this. */
	sub      lr, lr, #4

	/* No nesting -- this is a temporary stack. */
	ldr      sp, =irq_stack_hi
	stmfd    sp!, {r0,r1}       @ Push r0, r1

	/* Move special regs into r0, r1. */
	mrs      r0, spsr       @ set r0 = cpsr_prev
	mov      r1, lr         @ set r1 = lr_irq

	/* Switch to supervisor mode */
	msr      cpsr_c, #(PSR_MODE_SVC | PSR_IRQ)

	/* Diq the spsr_svc because svc-mode (SWI) entry code saves it with IRQ 
	 * disabled.  I feel so dirty doing this.  
	 * We need to diq spsr because we need atomic reload of pc and cpsr
	 * during the return sequence in case we came in from user mode.
	 */
	msr      spsr, r0   @ set spsr_svc = cpsr_prev (NOT cpsr_irq)

	/* Save lr, pop of r0, r1 from irq stack, and then put them all on the 
	 * svc stack along with other caller-save registers.
	 * The stack will look like:
	 * {r0-r3, r8, ip, lr, pc} of source
	 */
	stmfd    sp!, {r1}                  @ push lr_irq onto SVC stack
	ldr      r1, =irq_stack_lo
	ldm      r1, {r0, r1}               @ restore r0, r1
	stmfd    sp!, {r0-r3, r8, ip, lr}   @ push caller saved registers
                                        @ onto SVC stack. Note lr refers
                                        @ to lr_svc

	/* Recover user registers and save */
	mrs      r0, spsr               @ set r0 = cpsr_prev
	stmfd    sp, {r0, sp, lr}^      @ save cpsr_prev, sp_usr, lr_usr into
                                    @ SVC stack
	add      sp, sp, #-12           @ Increment sp_svc due to pushed
                                    @ vals.

	/* Set up r8 correctly with u-boot dispatch table. */
	ldr      ip, =uboot_exports
	ldr      r8, [ip]
	
	/* Call the IRQ handler in C. */
	bl      irq_handler_c

	/* Restore registers.  Also restore CPSR. */
	ldmfd    sp, {r0, sp, lr}^              @ set r0 = cpsr_usr.
                                            @ Restore sp_usr, lr_usr
	add      sp, sp, #12
	msr      spsr, r0                       @ set spsr_svc = cpsr_usr
	ldmfd    sp!, {r0-r3, r8, ip, lr, pc}^  @ Restore all registers.
                                            @ Note lr in reg list refers
                                            @ to lr_svc.
    
    @ Note: If context switch occured, then the restored registers will
    @ be from a different tcb kstack compared to the one that was active
    @ when the interrupt occured.

/*
 * We have exactly one IRQ save area.  This is shared across all tasks.  When an
 * IRQ is taken, this region is used as a temporary area to shuffle values across
 * the the SVC stack.  We don't want a full block IRQ stack, let alone an IRQ stack
 * per task.
 */
	.bss
	ALIGN8
GLOBAL(irq_stack_lo)
	.space 8
GLOBAL(irq_stack_hi)

