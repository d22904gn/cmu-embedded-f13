@ read.S: read sycall wrapper
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    10 Oct 2013

#include <bits/swi.h>

    .file   "read.S"
    .text

	.global read
read:
    swi     READ_SWI
    cmp     r0, #0      @ Check for syscall error.
    
    @ Subsequent instructions execute only if syscall returned an error.
    rsblt   r0, r0, #0      @ Make syscall return value positive
    strlt   r0, [pc, #-4]   @ Stuff return val into errno.
    .word   errno           @ errno's address
    mvnlt   r0, #0          @ Return -1
    
    @ Return to caller
    mov     pc, lr
