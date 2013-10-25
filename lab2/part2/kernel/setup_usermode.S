@ setup_usermode.S: Setups usermode from kernel
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    12 Oct 2013

#include <bits/swi.h>

    .file   "setup_usermode.S"
    .text
    .global setup_usermode
    .type   setup_usermode, %function
    
setup_usermode:
    @ Switch to user mode with IRQs & FIQs masked
    mrs     r2, cpsr
    bic     r2, r2, #0x1f           @ Clear mode bits.
    orr     r2, r2, #0xd0           @ Disable FIQs and IRQs and change to user
                                    @ mode by setting mode bits to 110 10000
    msr     cpsr_c, r2              @ Change to user mode.

    @ Setup a full descending user mode stack (with the stack top at 0xa3000000)
    ldr     sp, =0xa3000000

    @ Push U-Boot’s argc & argv on the user stack.
    @ Registers r0 and r1 contain the arguments to this function, which are
    @ U-Boot's argc and argv values respectively.
    push    {r0,r1}

    @ Jump to a loaded user program at address 0xa2000000.
    mov     lr, pc
    ldr     pc, =0xa2000000
    
    @ Exit via syscall when user returns. Easier to clean up code this way since
    @ all exit procedures are stored in the exit syscall.
    swi     EXIT_SWI
