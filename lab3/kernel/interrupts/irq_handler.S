/**
 * @file    irq_handler.S
 *
 * @brief   Custom IRQ handler.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    05 Nov 2013
 */

#include <asm.h>

    .file   "irq_handler.S"

FUNC(irq_handler)
    sub     sp, sp, #4          @ Save space on the stack for spsr
    push    {r0-r12, lr}        @ Save user registers and link register.

    @ Save spsr
    mrs     r2, spsr
    str     r2, [sp, #56]       @ Store spsr above the gp registers

    @ Call our C IRQ handler
    mov     lr, pc
    ldr     pc, =irq_handler_c

    @ Restore spsr
    ldr     r2, [sp, #56]
    msr     spsr, r2

    @ Restore old registers. Do not restore r0; it has the syscall return val.
    pop     {r0-r12, lr}

    @ Return to spsr state.
    movs    pc, lr
