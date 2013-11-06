/**
 * @file    exit.S
 *
 * @brief   exit() syscall
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    02 Nov 2013
 */

#include <asm.h>

    .file   "exit.S"
    
FUNC(exit)
    @ Save scratch registers
    push    {r0, r1}
    
    @ Restore U-Boot SWI handler
    
    ldr     r0, =uboot_swi_handler_addr @ Load SWI handler addr
    ldr     r1, =uboot_swi_instr1
    str     r1, [r0]                    @ Write back 1st instruction.
    add     r0, r0, #4
    ldr     r1, =uboot_swi_instr2
    str     r1, [r0]                    @ Write back 2nd instruction.
    
    
    @ Restore U-Boot IRQ handler
    
    @ Restore scratch registers.
    pop     {r0, r1}
    
    @ Cleanup the stack from main()
    ldr     r4, =uboot_sp
    ldr     sp, [r4]
    ldmia   sp!, {r4, r5, r6, r7, sl, pc}
