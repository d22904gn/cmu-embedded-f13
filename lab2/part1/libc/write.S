@ write.S: write sycall wrapper
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    10 Sep 2013

#include <bits/swi.h>

    .file   "write.S"
    .extern errno       @ Just for documentation. Ignored by GAS.
    .text

    .global write
write:
	swi     WRITE_SWI
    cmp     r0, #0      @ Check for syscall error.
    
    @ Subsequent instructions execute only if syscall returned an error.
    rsblt   r0, r0, #0  @ Make syscall return value positive
    ldrlt   r1, =errno  @ Load address of errno
    strlt   r0, [r1]    @ Stuff return val into errno.
    mvnlt   r0, #0      @ Return -1
