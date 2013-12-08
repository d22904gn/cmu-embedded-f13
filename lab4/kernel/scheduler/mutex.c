/** 
 * @file    mutex.c
 * 
 * @brief   Mutex implementation
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    12 Dec 2013
 */

#include <bits/errno.h>
#include <config.h>
#include <types.h>
#include <arm/interrupt.h>
#include <inline.h>
#include "scheduler.h"

// Readibility definitions
#define NO_OWNER NULL

// Allocate space for mutexes
mutex_t mutexes[OS_NUM_MUTEX];

// Track available mutexes
static int next_mutex = 0;

// HLP Emulation. Meh. This replaces the need to track the current
// priority ceiling since we are assigning each mutex a priority ceiling
// of 0 (Highest priority.) Since attempts at locking other mutexes from
// other tasks will always be denied due to rule 2(ii) in lecture slides
//, it is suffient to track priority ceiling with a pointer to the
// current task which is holding the mutex.
static tcb_t *curr_mutex_owner = NO_OWNER;

// HLP: Elevate task priority to comply with HLP
INLINE void elevate_prio(tcb_t *task) {
    /* Implement HLP by elevating current task to highest possible
     * priority while it has the mutex. Note that if this task goes
     * to sleep and another task acquires another mutex while it is
     * sleeping, then it will have to wait until that other task 
     * finishes/sleeps before this task can run again. (Since that
     * other task will also be elevated to the highest priority as 
     * well.)
     */
    
    // No need to do anything if curr task is already at the highest
    // priority.
    if (task->curr_prio == 0 || task->native_prio == 0) return;
    
    // If curr task is not the highest prio, then highest prio task
    // must be sleeping/waiting on a device or mutex. So it is safe 
    // to simply set curr task's priority to the highest priority.
    task->curr_prio = 0;
}


// Initialize mutexes
void mutex_init() {
    curr_mutex_owner = NO_OWNER;
    
    uint32_t i;
    for (i = 0; i < OS_NUM_MUTEX; i++) {
        mutexes[i].curr_owner = NO_OWNER;
        tcbqueue_init(&(mutexes[i].sleep_queue));
    }
}

// Creates a new mutex.
int mutex_create() {
    if (next_mutex >= OS_NUM_MUTEX) return -ENOMEM;
    
    return next_mutex++;
}

// Tries to let current task lock; if not, we put it in the sleep queue.
// Note that the sleep queue is a FIFO queue, not a priority queue.
int mutex_lock(int mutex_num) {
    // Sanity checks
    if (mutex_num >= next_mutex) return -EINVAL;
    
    // No mucking around with curr_tcb while we're accesing it!
    INT_ATOMIC_START;
    
    // Due to HLP, do not allow tasks to acquire mutexes when any mutex
    // (not necessarily the current task's desired mutex) is in use.
    // If mutex is unavailable, put the task on a sleep queue.
    if ((curr_mutex_owner == NO_OWNER
            || curr_mutex_owner->native_prio == curr_tcb->native_prio)
        && mutexes[mutex_num].curr_owner == NO_OWNER) {
        // Assign mutex to task.
        mutexes[mutex_num].curr_owner = curr_tcb;
        curr_tcb->locks_held++;
        
        // Elevate priority and disable lockings for HLP.
        elevate_prio(curr_tcb);
        curr_mutex_owner = curr_tcb;
    } else {
        // Make sure task does not acquire same mutex twice.
        if (mutexes[mutex_num].curr_owner->native_prio ==
            curr_tcb->native_prio) return -EDEADLOCK;
        
        // Add task to the sleep queue.
        tcbqueue_enqueue(&(mutexes[mutex_num].sleep_queue), curr_tcb);
        
        // Send the task to sleep.
        dispatch_sleep();
    }
    
    INT_ATOMIC_END;
    
    return 0;
}

// Unlocks the mutex for the current task.
int mutex_unlock(int mutex_num) {
    // Sanity checks
    if (mutex_num >= next_mutex
        || mutexes[mutex_num].curr_owner == NO_OWNER) return -EINVAL;
    
    INT_ATOMIC_START;
    if (mutexes[mutex_num].curr_owner->native_prio !=
        curr_tcb->native_prio) return -EPERM;
    
    // Release mutex from current owner.
    mutexes[mutex_num].curr_owner = NO_OWNER;
    curr_tcb->locks_held--;
    
    // We don't release mutexes to any other person until all the
    // mutexes held by the current task are released.
    if (curr_tcb->locks_held > 0) {
        return 0;
    }
    
    // De-elevate current task priority.
    curr_tcb->curr_prio = curr_tcb->native_prio;
    
    INT_ATOMIC_END;
    
    // Check if other tasks want the mutex, or are waiting on other
    // mutexes due to HLP. We select the next highest priority task at
    // the head of the mutexes sleep queues to run. Note that mutex
    // sleep queues are FIFO, but our HLP implementation is a priority
    // queue. (Naive solution would be to iterate through mutexes by
    // index, but using mutex index as a criteria for selecting tasks to
    // run doesn't make sense.)
    tcb_t* next_tcb = NULL;
    uint8_t highest_prio = IDLE_PRIO;
    int source_mutex = OS_NUM_MUTEX;
    
    // Linear search for highest priority task in all sleep queues.
    int i;
    for (i = 0; i < next_mutex; i++) {
        if (mutexes[i].sleep_queue.size != 0) {
            tcb_t* queue_tcb = 
                tcbqueue_poll(&(mutexes[i].sleep_queue));
            
            if (queue_tcb->curr_prio < highest_prio) {
                highest_prio = queue_tcb->curr_prio;
                next_tcb = queue_tcb;
                source_mutex = i;
            }
        }
    }
    
    // Give mutex to next task if we have found a waiting task.
    if (next_tcb != NULL) {
        // Remove task from mutex sleep queue first.
        tcbqueue_dequeue(&(mutexes[source_mutex].sleep_queue));
        
        // Give next task the mutex.
        mutexes[source_mutex].curr_owner = next_tcb;
        next_tcb->locks_held++;
        elevate_prio(next_tcb);
        curr_mutex_owner = next_tcb;
        
        INT_ATOMIC_START;
        runqueue_add(next_tcb, next_tcb->curr_prio);
        dispatch_save();
        INT_ATOMIC_END;
    } else {
        // Safe to allow locking again if no other task wants a mutex.
        curr_mutex_owner = NO_OWNER;
    }
    
    return 0;
}
