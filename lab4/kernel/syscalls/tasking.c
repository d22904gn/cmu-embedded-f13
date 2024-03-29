/**
 * @file    tasking.c
 *
 * @brief   Tasks related system calls.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#include <types.h>
#include <task.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>
#include "../scheduler/scheduler.h"

int task_create(task_t* tasks, size_t num_tasks) {
    // Sanity checks.
    if (num_tasks > OS_AVAIL_TASKS) return -EINVAL;
    
    /*
     * Assign schedule via RMA scheduling.
     */
    task_t* sorted_tasks[num_tasks];
    size_t i;
    
    // Prep
    for (i = 0; i < num_tasks; i++) {
        sorted_tasks[i] = tasks + i;
    }
    
    INT_ATOMIC_START;
    // Perform UB test.
    if (!assign_schedule(sorted_tasks, num_tasks)) return -EFAULT;
    
    // Init TCBs
    if (!allocate_tasks(sorted_tasks, num_tasks)) return -EFAULT;
    
    // Start device interrupt timer.
    reg_write(OSTMR_OSMR_ADDR(2),
        reg_read(OSTMR_OSCR_ADDR) + get_ticks(DEV_INT_PERIOD));
    
    // Start tasks
    dispatch_nosave();
    INT_ATOMIC_END;
    
    return 0;
}

int event_wait(unsigned int dev_num) {
    // Sanity checks
    if (dev_num >= NUM_DEVICES) return -EINVAL;
    
    // Prevent curr_tcb from being modified by device_update.
    INT_ATOMIC_START;
    
    // Prevent tasks from waiting if they have a mutex.
    if (curr_tcb->locks_held != 0) return -EHOLDSLOCK;
    
    // Restore device priority to its native priority.
    curr_tcb->curr_prio = curr_tcb->native_prio;
    
    // Put calling task on the device queue.
    dev_enqueue(curr_tcb, dev_num);
    
    // Sleep current task.
    dispatch_sleep();
    
    INT_ATOMIC_END;
    
    return 0;
}
