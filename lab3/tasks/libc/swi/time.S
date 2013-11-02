/**
 * @file    time.S
 *
 * @brief   time sycall wrapper
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    02 Nov 2013
 */

#include <asm.h>
#include <bits/swi.h>

	.file "time.S"

FUNC(time)
	swi     TIME_SWI
    
    @ Return to caller
    mov     pc, lr
