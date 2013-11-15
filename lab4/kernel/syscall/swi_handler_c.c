/**
 * @file   swi_handler.c
 *
 * @brief  Custom SWI handler in C.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    14 Nov 2013
 */

#include <bits/swi.h>
#include <exports.h>
#include <syscall.h>

void swi_handler_c(uint32_t swi_num, int *r) {
    switch (swi_num) {
    case READ_SWI:
        read_syscall(r[0], (void*) r[1], (size_t) r[2]);
        break;
    case WRITE_SWI:
        write_syscall(r[0], (void*) r[1], (size_t) r[2]);
        break;
    case TIME_SWI:
        time_syscall();
        break;
    case SLEEP_SWI:
        sleep_syscall((unsigned long) r[0]);
        break;
    default:
        printf("Illegal SWI number: %x\n", swi_num);
    }
}
