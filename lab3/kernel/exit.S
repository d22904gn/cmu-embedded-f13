@ exit.S: exit() syscall
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    12 Oct 2013

    .file   "exit.S"
    .text
    .global exit
    .type   exit, %function
    
exit:
    @ Restore U-Boot SWI handler
    push    {r0, r1}
    ldr     r0, =uboot_handler_addr     @ Load SWI handler addr
    ldr     r1, =uboot_swi_instr1
    str     r1, [r0]                    @ Write back 1st instruction.
    add     r0, r0, #4
    ldr     r1, =uboot_swi_instr2
    str     r1, [r0]                    @ Write back 2nd instruction.
    pop     {r0, r1}
    
    @ Cleanup the stack from main()
    ldr     r4, =uboot_sp
    ldr     sp, [r4]
    add     sp, sp, #4
    pop     {r4, r5, lr}
    
    @ Go back to U-Boot
    bx      lr
