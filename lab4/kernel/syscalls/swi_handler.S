/**
 * @file    swi_handler.S
 *
 * @brief   Custom SWI handler
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    18 Nov 2013
 */

#include <asm.h>
#include <arm/psr.h>

    .file   "swi_handler.S"

FUNC(swi_handler)
    push    {r0-r8, ip, lr}        @ Save argument regs, r8, ip & lr
    
    @ Restore U-Boot export table.
    ldr     r0, =uboot_exports
    ldr     r8, [r0]
    
    @ Enable interrupts.
    msr     cpsr_c, #(PSR_MODE_SVC | PSR_FIQ)

    @ Call our C SWI handler
    ldr     r0, [lr, #-4]       @ Pass in swi number
    bic     r0, r0, #0xff000000
    mov     r1, sp              @ Pass in saved register array
    mov     lr, pc
    ldr     pc, =swi_handler_c  @ Jump to our C SWI handler.

    @ Restore old registers.
    @ Do not restore r0; it has the syscall return val.
    add     sp, sp, #4      @ Bypass r0 in the stack.
    pop     {r1-r8, ip, lr}

    @ Return to spsr state.
    movs    pc, lr
