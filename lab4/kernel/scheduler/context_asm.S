/** @file context_asm.S
 *
 * @brief Contains ASM context related routines.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @author Kartik Subramanian
 * @date   2008-11-21
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
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

/* r0 points to the target context, r1 to the current context.
   r2 is 1 iff we need to call launch_task. */
/* Note: Safe to clobber r0-r3 */
FUNC(ctx_switch_full)
    @ Save current context
    stmia   r0, {r4-r11, sp, lr}
    
    @ Call ctx_switch_half.
    mov     r1, r2
    b       ctx_switch_half
    
/* r0 points to the target context. r1 is 1 iff we need to call
   launch_task. */
/* Note: Safe to clobber r0-r3 */
FUNC(ctx_switch_half)
    @ Check if we need to call launch_task (I.e. fresh task)
    cmp     r1, #0
    bne     launch_task
    
    @ If not a fresh task, switch in new context.
    ldmia   r0, {r4-r11, sp, lr}
    mov     pc, lr
