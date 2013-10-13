@ exit.S: exit() syscall
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    12 Oct 2013

    .file   "exit.S"
    .text
    .global exit
    .type   exit, %function
    
exit:
    @ Restore U-Boot SWI handler

    @ Go back to U-Boot
    mov    pc, lr
