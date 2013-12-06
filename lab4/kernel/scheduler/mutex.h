/**
 * @file    mutex.h
 *
 * @brief   Mutex-related declarations.
 *          Based on lab4 handout code.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#include <config.h>
#include <types.h>
#include <task.h>
#include "tcbqueue.h"

#ifndef MUTEX_H
#define MUTEX_H

struct mutex {
    tcb_t  *curr_owner;
    tcb_queue_t     sleep_queue;
};
typedef struct mutex mutex_t;

void mutex_init();
int mutex_create();
int mutex_lock(int mutex_num);
int mutex_unlock(int mutex_num);

#endif /* MUTEX_H */
