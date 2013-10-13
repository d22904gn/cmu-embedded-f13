@ swi_handler.S: Custom SWI handler
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    12 Oct 2013

    .file   "swi_handler.S"
    .text
    .global swi_handler
    .type   swi_handler, %function

swi_handler:
    @ Save user registers and link register.
	stmfd   sp!, {r0-r12, lr}
    
    @ Call our C SWI handler
    ldr     r0, [lr, #-4]       @ Pass in swi number
    bic     r0, r0, #0xff000000
    mov     r1, sp              @ Pass in saved register array
    mov     lr, pc
    ldr     pc, =swi_handler_c  @ Jump to our C SWI handler.
    
    @ Restore user registers. Do not restore r0; it has the syscall return val.
    add     sp, sp, #4          @ Bypass r0 in the stack.
    ldmfd   sp!, {r1-r12, lr}
    
    @ Return to user mode.
    movs    pc, lr
