/**
 * @file    enter_usermode.S
 *
 * @brief   Enters usermode from kernel main.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    14 Nov 2013
 */

#include <asm.h>
#include <arm/psr.h>
#include <config.h>

    .file   "enter_usermode.S"

FUNC(enter_usermode)
    @ Switch to user mode with IRQs and FIQs disabled.
    msr     cpsr_c, #(PSR_MODE_USR | PSR_IRQ | PSR_FIQ)

    @ Setup a full descending user mode stack
    ldr     sp, =USR_STACK

    @ Push U-Boot's argc & argv array on the user stack.
    @ Registers r0 and r1 contain the arguments to this function, which are
    @ U-Boot's argc and argv values respectively.
    
    @ Push null terminator onto the stack first.
    mov     r2, #0
    push    {r2}
    
    @ Start copying stuff onto the stack.
    mov     r2, r0, LSL #2          @ r2 = argc * 4 = number of bytes in argv
    add     r1, r1, r2              @ r1 = r1 + r2 = argv + 4*len(argv)
    mov     r3, r0                  @ r3 = argc
    
stack_loop:
    ldr     r2, [r1, #-4]!          @ Load argv[i] into r2 (desecending order)
    push    {r2}                    @ Push argv[i] onto the stack
    subs    r3, r3, #1
    blne    stack_loop
    
    @ Push argc onto the stack.
    push    {r0}

    @ Jump to a loaded user program.
    mov     lr, pc
    ldr     pc, =USR_START_ADDR
