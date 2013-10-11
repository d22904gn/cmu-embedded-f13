@ crt0.S: Execution startup routine
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    10 Sep 2013

    .file   "crt0.S"
    .text

    .global _start
_start:
    @ Load argv** and argc into arg registers
    ldr     r0, [sp]    @ argc
    add     r1, sp, #4  @ argv**
    
    @ Call main via direct PC math-fu. (Since we don't know where main() is.)
    mov     lr, pc  @ Update lr manually with our return address. Note it's 
                    @ <instr addr> + 8 = pc since next instruction is our branch
    ldr     pc, =main
    
    @ Call exit via math-fu again.
    ldr     pc, =exit
