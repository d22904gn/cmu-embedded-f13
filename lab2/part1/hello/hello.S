@ hello.S: Hello world! in assembly
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    10 Sep 2013

#include <bits/swi.h>
#include <bits/fileno.h>

    .file	"hello.S"
    .text

    .global	_start
_start:
    mov r0, STDOUT_FILENO
    
    swi WRITE_SWI
    mov r0, #0
    swi EXIT_SWI
