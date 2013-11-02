/**
 * @file    exit.S
 *
 * @brief   exit sycall wrapper
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    02 Nov 2013
 */

#include <asm.h>
#include <bits/swi.h>

	.file "exit.S"

FUNC(exit)
    swi EXIT_SWI
