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
    @ ** NOTE: USING OABI CONVENTION **

    @ Restore U-Boot SWI handler
    strfd   sp!, {r0, r1}  @ Save scratch registers
    ldr     r0, [sp, #8]   @ Load SWI handler addr
    ldr     r1, [sp, #16]
    str     r1, [r0]       @ Write back 1st instruction.
    ldr     r1, [sp, #12]
    str     r1, [r0, #4]   @ Write back 2nd instruction.    
    ldmfd   sp!, {r0, r1}  @ Restore scratch registers
    add     sp, sp, #12    @ Remove stored U-Boot code from stack

    @ Go back to U-Boot
    mov    pc, lr
