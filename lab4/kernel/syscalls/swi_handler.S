/**
 * @file    swi_handler.S
 *
 * @brief   Custom SWI handler
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    20 Nov 2013
 */

#include <asm.h>
#include <arm/psr.h>

    .file   "swi_handler.S"

/* The SWI handler stack looks like IRQ handler stack so that context
 * switching can be done in the case where:
 *
 * 1. Task 1 calls event_wait(). SWI handler puts stuff on stack.
 *    event_wait() calls dispatch_sleep(). Switches stack to the next
 *    task's stack (Task 2).
 * 2. Task 2 runs, receives interrupt. IRQ handler puts stuff on stack.
 *    dev_update() calls dispatch_save(). This switches stack to task
 *    1's stack.
 * 3. IRQ handler unrolls stack. If SWI handler stack looks different 
 *    from the way the IRQ handler setups the stack, then the program
 *    will derp.
 */

FUNC(swi_handler)
    push    {lr}                    @ For irq_handler emulation
    push    {r0-r3, r8, ip, lr}     @ Save caller saved regs.
    
    @ Save spsr. Saving user registers to emulate irq_handler.
    mrs     r0, spsr
    stmfd   sp, {r0, sp, lr}^
    sub     sp, sp, #12
    
    @ Restore U-Boot export table.
    ldr     r0, =uboot_exports
    ldr     r8, [r0]
    
    @ Enable interrupts.
    msr     cpsr_c, #(PSR_MODE_SVC | PSR_FIQ)

    @ Call our C SWI handler
    ldr     r0, [lr, #-4]       @ Pass in swi number
    bic     r0, r0, #0xff000000
    add     r1, sp, #12         @ Pass in saved register array
                                @ Offset due to irq_handler emulation.
    mov     lr, pc
    ldr     pc, =swi_handler_c  @ Jump to our C SWI handler.

    @ Restore spsr. Restoring user sp & lr due to irq_handler emulation.
    @ We do not restore to r0 as that contains the syscall return val.
    ldmfd   sp, {r1, sp, lr}^
    add     sp, sp, #12
    msr     spsr, r1
    
    @ Restore old registers and return.
    add     sp, sp, #4                      @ Bypass r0 in the stack.
    ldmfd   sp!, {r1-r3, r8, ip, lr, pc}^
