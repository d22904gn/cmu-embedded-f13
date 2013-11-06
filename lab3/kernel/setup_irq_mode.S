/**
 * @file    setup_irq_mode.S
 *
 * @brief   Setups IRQ mode stuff.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    06 Nov 2013
 */

#include <asm.h>

#define IRQ_STACK_START 0xa3ffffff

    .file   "setup_irq_mode.S"

FUNC(setup_irq_mode)
    @ Switch to IRQ mode with IRQs & FIQs masked
    mrs     r0, cpsr
    bic     r1, r0, #0x1f
    orr     r1, r1, #0xd2
    msr     cpsr_c, r1
    
    @ Setup the IRQ stack at the end of DRAM.
    ldr     sp, =IRQ_STACK_START
    
    @ Return to supervisor mode.
    msr     cpsr_c, r0
    
    @ Go back to main()
    mov     pc, lr
