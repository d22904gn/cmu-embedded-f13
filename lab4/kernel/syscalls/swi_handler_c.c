/**
 * @file   swi_handler.c
 *
 * @brief  Custom SWI handler in C.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    03 Nov 2013
 */

#include <arm/interrupt.h>
#include <bits/swi.h>
#include <exports.h>
#include "syscalls.h"

void swi_handler_c(uint32_t swi_num, int *r) {
    switch (swi_num) {
    case READ_SWI:
        read(r[0], (void*) r[1], (size_t) r[2]);
        break;
    case WRITE_SWI:
        write(r[0], (void*) r[1], (size_t) r[2]);
        break;
    case TIME_SWI:
        time();
        break;
    case SLEEP_SWI:
        sleep((unsigned long) r[0]);
        break;
    case CREATE_SWI:
        task_create((task_t*) r[0], (size_t) r[1]);
        break;
    case MUTEX_CREATE:
        mutex_create();
        break;
    case MUTEX_LOCK:
        mutex_lock(r[0]);
        break;
    case MUTEX_UNLOCK:
        mutex_unlock(r[0]);
        break;
    case EVENT_WAIT:
        event_wait(r[0]);
        break;
    default:
        printf("Illegal SWI number: %x\n", swi_num);
        disable_interrupts();
        while(1);
    }
}
