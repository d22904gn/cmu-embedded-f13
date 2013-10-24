@ start.S: Kernel startup routine
@
@ Author: Mike Kasick <mkasick@andrew.cmu.edu>
@ Date:   Tue, 23 Oct 2007 11:20:33 -0400

    .file   "setup_usermode.S"
    .text
    .global setup_usermode
    .type   setup_usermode, %function
setup_usermode:
    @ Function supports interworking.
    @ args = 0, pretend = 0, frame = 0
    @ frame_needed = 1, uses_anonymous_args = 0

    @ Switch to user mode with IRQs & FIQs masked
    MRS     r2, cpsr
    BIC     r2, r2, #0x1f           @ Clear mode bits.
    ORR     r2, r2, #0xd0           @ Disable FIQs and IRQs and change to user
                                    @ mode by setting mode bits to 110 10000
    MSR     cpsr_c, r2              @ Change to user mode.

    @ Setup a full descending user mode stack (with the stack top at 0xa3000000)
    LDR     r13, =0xa3000000

    @ Push U-Boot’s argc & argv on the user stack.
    @ Registers r0 and r1 contain the arguments to this function, which are
    @ U-Boot's argc and argv values respectively.
    push {r0,r1}

    @  Jump to a loaded user program at address 0xa2000000.
    LDR    pc, =0xa2000000

    @ .size   setup_usermode, .-setup_usermode
    @ .section    .rodata
    @ .align  2
