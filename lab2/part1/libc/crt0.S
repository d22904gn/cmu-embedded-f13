@ crt0.S: Execution startup routine
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    10 Oct 2013

    .file   "crt0.S"
    .text

    .global _start
_start:
    @ Load argv** and argc into arg registers
    ldr     r0, [sp]    @ Load argc
    add     r1, sp, #4  @ Load argv**
    
    @ Call main
    mov     lr, pc
    ldr     pc, =main
    
    @ Call exit
    ldr     pc, =exit
