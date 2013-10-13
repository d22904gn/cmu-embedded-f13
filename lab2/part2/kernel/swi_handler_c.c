/*
 * swi_handler.c: Custom SWI handler
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <exports.h>
#include "syscalls.h"

void swi_handler_c(int swi_num, void *r) {
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
