/**
 * @file    exit.S
 *
 * @brief   exit() "syscall"
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    02 Nov 2013
 */

#include <asm.h>

    .file   "exit.S"
    
FUNC(exit)
    @ Restore U-Boot SWI handler
    ldr     r1, =uboot_swi_handler_addr @ Load SWI handler addr
    ldr     r2, =uboot_swi_instr1
    str     r2, [r1]                    @ Write back 1st instruction.
    add     r1, r1, #4
    ldr     r2, =uboot_swi_instr2
    str     r2, [r1]                    @ Write back 2nd instruction.
    
    @ Restore U-Boot IRQ handler
    ldr     r1, =uboot_irq_handler_addr
    ldr     r2, =uboot_irq_instr1
    str     r2, [r1]
    add     r1, r1, #4
    ldr     r2, =uboot_irq_instr2
    str     r2, [r1]
    
    @ Cleanup the stack from main()
    @ Safe to clobber r4 since we are restoring it.
    ldr     r4, =uboot_sp
    ldr     sp, [r4]
    ldmia   sp!, {r4, r5, r6, r7, sl, pc}
