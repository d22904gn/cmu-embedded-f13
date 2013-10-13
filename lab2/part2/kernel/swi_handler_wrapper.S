@ swi_handler_wrapper.S: ASM wrapper for our SWI handler
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    12 Oct 2013

    .file   "swi_handler_wrapper.S"
    .text
    .global swi_handler_wrapper
    .type   swi_handler_wrapper, %function
    
    @ Save user registers and original r0.
	stmfd   sp!, {r0-r12, lr}
    
    @ Load addr of saved registers into r1/2nd arg.
    mov     r1, sp
    
    @ Load SWI number into r0/first arg.
    ldr     r0, [lr, #-4]
    bic     r0, r0, #0xff000000
    
    @ Jump to our C SWI handler.
    ldr     pc, [pc, #-4]
    .word   swi_handler
    
    @ Restore user registers. Do not restore r0; it contains the syscall return.
    add     sp, sp, #4
    ldmfd   sp!, {r1-r12, lr}
    
    @ Return to user mode.
    movs    pc, lr
