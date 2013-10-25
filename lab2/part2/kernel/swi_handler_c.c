/*
 * swi_handler.c: Custom SWI handler in C.
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <exports.h>
#include "common.h"
#include "syscalls.h"

void swi_handler_c(uint swi_num, int *r) {
    switch (swi_num) {
    case EXIT_SWI:
        exit(r[0]);
    case READ_SWI:
        read(r[0], (void*) r[1], (size_t) r[2]);
        break;
    case WRITE_SWI:
        write(r[0], (void*) r[1], (size_t) r[2]);
        break;
    default:
        puts("Illegal SWI number!\n");
        printf("Illegal swi_num: %x", swi_num);
        exit(0x0badc0de);
    }
}
