/**
 * @file    disable_interrupts.S
 *
 * @brief   Disable interrupts
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    10 Nov 2013
 */

#include <asm.h>

    .file   "disable_interrupts.S"

FUNC(disable_interrupts)
    mrs     r0, cpsr
    orr     r0, r0, #0x80
    msr     cpsr_c, r0
    mov     pc, lr
