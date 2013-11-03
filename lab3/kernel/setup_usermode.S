/**
 * @file    setup_usermode.S
 *
 * @brief   Setups usermode from kernel main.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    02 Nov 2013
 */

#include <asm.h>
#include <bits/swi.h>

#define USR_STACK_START 0xa3000000
#define USR_PROG_ENTRY  0xa0000000

    .file   "setup_usermode.S"

FUNC(setup_usermode)
    @ Switch to user mode with IRQs & FIQs masked
    mrs     r2, cpsr
    bic     r2, r2, #0x1f           @ Clear mode bits.
    orr     r2, r2, #0xd0           @ Disable FIQs and IRQs and change to user
                                    @ mode by setting mode bits to 110 10000
    msr     cpsr_c, r2              @ Change to user mode.

    @ Setup a full descending user mode stack
    ldr     sp, =USR_STACK_START

    @ Push U-Boot's argc & argv array on the user stack.
    @ Registers r0 and r1 contain the arguments to this function, which are
    @ U-Boot's argc and argv values respectively.
    mov     r2, r0, LSL #2          @ r2 = argc * 4 = number of bytes in argv
    add     r1, r1, r2              @ r1 = r1 + r2 = argv + 4*len(argv)
    mov     r3, r0                  @ r3 = argc

stack_loop:
    ldr     r2, [r1, #-4]!          @ Load argv[i] into r2 (desecending order)
    push    {r2}                    @ Push argv[i] onto the stack
    subs    r3, r3, #1
    blne    stack_loop

    push    {r0}                    @ Push argc onto the stack.

    @ Jump to a loaded user program.
    mov     lr, pc
    ldr     pc, =USR_PROG_ENTRY

    @ Exit via syscall when user returns. Easier to clean up code this way since
    @ all exit procedures are stored in the exit syscall.
    swi     EXIT_SWI
