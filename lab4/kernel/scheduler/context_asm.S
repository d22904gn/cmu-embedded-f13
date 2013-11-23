/** @file context_asm.S
 *
 * @brief Contains ASM context switch routines.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @author Kartik Subramanian
 * @date   2008-11-21
 */

    .file "context_asm.S"

#include <asm.h>
#include <arm/psr.h>

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * r7 contains the kernel mode stack pointer.
 * Upon completion, we should be in user mode.
 */

FUNC(launch_task)
    mov     sp, r7
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
	mov     pc, r4


/* r0 points to the target context, r1 to the current context. */
/* Note: Safe to clobber r0-r3 */
FUNC(ctx_switch_full)
    @ Save old context.
    @ -- Save kernel stack
    
    @ -- Save user registers
    
    
    @ Switch to new context
    @ -- Check if we need to call launch_task (I.e. fresh task)
    @ If yes:
    @ -- 
    
    @ If no:
    @ -- Restore kernel stack
    @ -- Restore user registers
    
    

/* r0 points to the target context, r1 to the current context. */
/* Note: Safe to clobber r0-r3 */
FUNC(ctx_switch_half)
    @ Save old context.
    @ -- Save kernel stack
    
    @ -- Save user registers
    
    
    @ Switch to new context
    @ -- Check if we need to call launch_task (I.e. fresh task)
    @ If yes:
    @ -- 
    
    @ If no:
    @ -- Restore kernel stack
    @ -- Restore user registers
    
    
    
