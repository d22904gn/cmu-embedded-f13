@ hello.S: Hello world! in assembly
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    10 Oct 2013

#include <bits/swi.h>
#include <bits/fileno.h>

    .file	"hello.S"
    .text

    .global	_start
_start:
    mov r0, #STDOUT_FILENO  @ Load fd for stdout
    ldr r1, =hello_str      @ Load string address
    mov r2, #hello_size     @ Load string length
    swi WRITE_SWI           @ Write hello world to stdout
    mov r0, #0
    swi EXIT_SWI            @ Exit with 0.

hello_str:
    .ascii  "Hello world!\n"
    .set    hello_size, .-hello_str
