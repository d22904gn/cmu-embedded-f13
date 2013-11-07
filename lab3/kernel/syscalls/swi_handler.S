/**
 * @file    swi_handler.S
 *
 * @brief   Custom SWI handler
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    02 Nov 2013
 */

#include <asm.h>

    .file   "swi_handler.S"

FUNC(swi_handler)
    sub     sp, sp, #4          @ Save space on the stack for spsr
    push    {r0-r12, lr}        @ Save user registers and link register.
    
    @ Restore U-Boot export table.
    ldr     r2, =uboot_exports
    ldr     r8, [r2]
    
    @ Enable interrupts.
    mrs     r2, cpsr
    bic     r2, r2, #0x80
    msr     cpsr_c, r2

    @ Save spsr
    mrs     r2, spsr
    str     r2, [sp, #56]       @ Store spsr above the gp registers

    @ Call our C SWI handler
    ldr     r0, [lr, #-4]       @ Pass in swi number
    bic     r0, r0, #0xff000000
    mov     r1, sp              @ Pass in saved register array
    mov     lr, pc
    ldr     pc, =swi_handler_c  @ Jump to our C SWI handler.
    
    @ Restore spsr
    ldr     r2, [sp, #56]
    msr     spsr, r2

    @ Restore old registers. Do not restore r0; it has the syscall return val.
    add     sp, sp, #4          @ Bypass r0 in the stack.
    pop     {r1-r12, lr}

    @ Return to spsr state.
    movs    pc, lr
