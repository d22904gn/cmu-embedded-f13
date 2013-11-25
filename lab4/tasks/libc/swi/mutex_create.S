/**
 * @file    mutex_create.S
 *
 * @brief   mutex_create sycall wrapper
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_create.S"

FUNC(mutex_create)
    swi     MUTEX_CREATE
    cmp     r0, #0      @ Check for syscall error.
    
    @ Subsequent instructions execute only if syscall returned an error.
    rsblt   r0, r0, #0      @ Make syscall return value positive
    ldrlt   r1, =errno
    strlt   r0, [r1]        @ Set errno = |return val|
    mvnlt   r0, #0          @ Return -1
    
    @ Return to caller
    mov     pc, lr

