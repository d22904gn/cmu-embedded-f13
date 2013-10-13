@ kernel.S: Kernel main entry point.
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    12 Oct 2013

#include <bits/swi.h>

    .file   "kernel.S"
    .text
    .global _start
    
_start:
    @ ** NOTE: USING APCS CONVENTION **
    
    @ Install our SWI handler
    @ -- Store contents of SWI vector into r2
    mov     r2, #0x8
    ldr     r2, [r2]
    
    @ -- Extract immediate from SWI vector into r3. (Faster to do this first.)
    ldr     r3, =0xfffff000
    bic     r3, r2, r3
    
    @ -- Check that SWI vector contains a valid instruction.
    ldr     ip, 0xe59ff000  @ Opcode for ldr [pc, #0]
    eor     r2, r2, r3      @ Clear out immediate value
    cmp     r2, ip
    
    @ -- Exit if SWI vector instruction is not valid.
    ldrne   r0, =bad_swi_str
    ldrne   lr, pc
    ldrne   pc, =puts           @ Print out error message 
    ldrne   r0, =0x0badc0de
    movne   pc, lr              @ Return to U-Boot
    
    @ -- Save U-Boot SWI handler
    ldr     r2, [r3, #16]   @ r2 = U-Boot SWI handler addr
    ldr     r3, [r2]
    strfd   sp!, {r3}       @ Store 1st instr of U-Boot handler
    ldr     r3, [r2, #4]
    strfd   sp!, {r3, r2}   @ Store 2nd instr & addr of U-Boot handler
    
    @ -- Hijack U-Boot SWI handler with our SWI handler
    ldr     r3, =0xe51ff004     @ r3 = Opcode for ldr pc, [pc, #-4]
    str     r3, [r2]            @ Overwrite 1st instr of U-Boot handler
    ldr     r3, =swi_handler    @ r3 = Our SWI handler addr
    str     r3, [r2, #4]        @ Overwrite 2nd instr of U-Boot handler
    
    @ Setup user stack and initial arguments (argc, argv)
    msr     CPSR_c, #0xD0       @ Switch to user mode
    
    @ Load user program
    
    @ Cleanup after user program and exit.
    swi #EXIT_SWI
    
bad_swi_str:
    .string "Bad SWI Vector!\n"
