/*
 * kernel.c: Kernel main (entry) function
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <bits/swi.h>
#include <exports.h> 
#include "syscalls.c"

// External functions.
extern void hijack_swi();           // Gumstick SWI hijacker. (ASM)
extern void restore_swi();          // Gumstick SWI restorer. (ASM)
extern void swi_handler_wrapper();  // Wrapper for our swi handler (ASM)

// Our custom SWI handler
void swi_handler(int swi_num, void *r) {
    switch (swi_num) {
    case EXIT_SWI:
        exit(r[0]);
    case READ_SWI:
        read(r[0], r[1], r[2]);
    case WRITE_SWI:
        write(r[0], r[1], r[2])
    default:
        puts("Illegal SWI number!\n");
        exit(0x0badc0de);
    }
}

int main(int argc, char *argv[]) {
    hijack_swi(); // Install SWI Handler.
    
    // Setup user stack.
    
    
    restore_swi(); // Restore old gumstick SWI handler.
}
