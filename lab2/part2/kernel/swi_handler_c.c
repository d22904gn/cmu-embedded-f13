/*
 * swi_handler.c: Custom SWI handler
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

//#include <exports.h>
//#include "syscalls.h"
#include "swi_handler_c.h"


//extern void swi_handler_c(int swi_num, void* r);

void swi_handler_c(int swi_num, int *r) {
    puts("IN SWI HANDLER C\n");
    switch (swi_num) {
    case EXIT_SWI:
        puts("EXITING!\n");
        exit(r[0]);
    case READ_SWI:
//      read(r[0], (void*) r[1], r[2]);
        puts("READING!\n"); // PLACEHOLDER
    case WRITE_SWI:
//      write(r[0], (void*) r[1], r[2]);
        puts("WRITING!\n"); // PLACEHOLDER
    default:
        puts("Illegal SWI number!\n");
        printf("Illegal swi_num: %x", swi_num);
        exit(0x0badc0de);
    }
}
