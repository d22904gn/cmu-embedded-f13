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
    
    @ Save spsr
    mrs     r1, spsr
    str     r1, [sp, #56]       @ Store spsr above the gp registers
    
    @ Restore U-Boot export table.
    ldr     r1, =uboot_exports
    ldr     r8, [r1]
    
    @ Enable interrupts.
    mrs     r1, cpsr
    bic     r1, r1, #0x80
    msr     cpsr_c, r1

    @ Call our C SWI handler
    ldr     r0, [lr, #-4]       @ Pass in swi number
    bic     r0, r0, #0xff000000
    mov     r1, sp              @ Pass in saved register array
    mov     lr, pc
    ldr     pc, =swi_handler_c  @ Jump to our C SWI handler.
    
    @ Restore spsr
    ldr     r1, [sp, #56]
    msr     spsr, r1

    @ Restore old registers.
    @ Do not restore r0; it has the syscall return val.
    add     sp, sp, #4      @ Bypass r0 in the stack.
    pop     {r1-r12, lr}
    add     sp, sp, #4      @ Bypass space for spsr.

    @ Return to spsr state.
    movs    pc, lr
