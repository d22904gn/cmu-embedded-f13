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
    @ Save old registers and spsr.
    sub     sp, sp, #4
    push    {r0-r12, lr}
    mrs     r0, spsr
    str     r0, [sp, #56]

    @ Call our C IRQ handler
    mov     lr, pc
    ldr     pc, =irq_handler_c
    
    @ Restore spsr and registers
    ldr     r0, [sp, #56]
    msr     spsr, r0
    pop     {r0-r12, lr}
    add     sp, sp, #4      @ Bypass space for spsr.

    @ Return to spsr state.
    subs    pc, lr, #4
