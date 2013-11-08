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
    @ Save spsr and link register
    mrs     r0, spsr
    push    {r0, lr}
    
    @ XXX: Restore export table if you intend to use U-Boot exports!
    @      Remember to save r8 before loading the export table back.
    
    @ Safe to re-enable interrupts here.
    @ XXX: For later labs

    @ Call our C IRQ handler
    mov     lr, pc
    ldr     pc, =irq_handler_c
    
    @ Mask interrupts before cleaning up.
    @ XXX: For later labs

    @ Restore spsr and lr
    pop     {r0}
    msr     spsr, r0
    pop     {lr}

    @ Return to spsr state.
    movs    pc, lr
