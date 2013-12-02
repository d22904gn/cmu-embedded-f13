/** 
 * @file    mutex.c
 * 
 * @brief   Mutex implementation
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#include <bits/errno.h>
#include <config.h>
#include <types.h>
#include <arm/interrupt.h>
#include "scheduler.h"

// Allocate space for mutexes
mutex_t mutexes[OS_NUM_MUTEX];

// Track available mutexes
static int next_mutex = 0;

// Initialize mutexes
void mutex_init() {
    uint32_t i;
    
    for (i = 0; i < OS_NUM_MUTEX; i++) {
        mutexes[i].is_available = TRUE;
        mutexes[i].curr_owner = 0;
        tcbqueue_init(&(mutexes[i].sleep_queue));
    }
}

// Creates a new mutex.
int mutex_create() {
    if (next_mutex >= OS_NUM_MUTEX) return -ENOMEM;
    
    return next_mutex++;
}

// Tries to let current task lock; if not, we put it in the sleep queue.
int mutex_lock(int mutex_num) {
    // Sanity checks
    if (mutex_num >= next_mutex) return -EINVAL;
    
    // If mutex is unavailable, put the task on a sleep queue.
    if (!mutexes[mutex_num].is_available) {
        // Make sure task does not acquire same mutex twice.
        if (mutexes[mutex_num].curr_owner->native_prio ==
            curr_tcb->native_prio) return -EDEADLOCK;
        
        // Add task to the sleep queue.
        tcbqueue_enqueue(&(mutexes[mutex_num].sleep_queue), curr_tcb);
        
        // Send the task to sleep.
        dispatch_sleep();
    } else {
        mutexes[mutex_num].is_available = FALSE;
        mutexes[mutex_num].curr_owner = curr_tcb;
        curr_tcb->holds_lock = TRUE;
    }
    
    return 0;
}

// Unlocks the mutex for the current task.
int mutex_unlock(int mutex_num) {
    // Sanity checks
    if (mutex_num >= next_mutex ||
        mutexes[mutex_num].is_available) return -EINVAL;
    if (mutexes[mutex_num].curr_owner->native_prio !=
        curr_tcb->native_prio) return -EPERM;
    
    // Mark the current owner as not holding a mutex.
    mutexes[mutex_num].curr_owner->holds_lock = FALSE;
    
    // Check if other tasks want the mutex.
    if (mutexes[mutex_num].sleep_queue.size != 0) {
        tcb_t* next_tcb = 
            tcbqueue_dequeue(&(mutexes[mutex_num].sleep_queue));
        
        // Give next task the mutex.
        mutexes[mutex_num].curr_owner = next_tcb;
        mutexes[mutex_num].curr_owner->holds_lock = TRUE;
        
        INT_ATOMIC_START;
        runqueue_add(next_tcb, next_tcb->curr_prio);
        INT_ATOMIC_END;
        
        /* If the next task has a higher priority than the current task,
         * context switch to it immediately. */
        if (is_higher_prio(next_tcb)) dispatch_save();
    } else {
        // Safe to release mutex if no other tasks want it currently.
        mutexes[mutex_num].curr_owner = 0;
        mutexes[mutex_num].is_available = TRUE;
    }
    
    return 0;
}
