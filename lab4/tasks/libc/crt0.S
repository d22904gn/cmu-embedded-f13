/** 
 * @file    crt0.S
 *
 * @brief   Execution startup routine
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    02 Nov 2013
 */

#include <asm.h>

	.file "crt0.S"

FUNC(_start)
	@ Load argv** and argc into arg registers
    ldr     r0, [sp]    @ Load argc
    add     r1, sp, #4  @ Load argv**
    
    @ Call main
    mov     lr, pc
    ldr     pc, =main
    
    @ Infinite loop after main returns.
    sub     pc, pc, #8;
