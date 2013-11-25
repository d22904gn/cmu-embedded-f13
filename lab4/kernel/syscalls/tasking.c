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
#include "../scheduler/scheduler.h"
#include "../scheduler/devices.h"

int task_create(task_t* tasks, size_t num_tasks) {
    // Sanity checks.
    if (num_tasks > OS_AVAIL_TASKS) return -EINVAL;
    
    /*
     * Sort tasks for RMA scheduling
     */
    task_t* sorted_tasks[num_tasks];
    size_t i, j, min_idx;
    
    // Selection sort prep
    for (i = 0; i < num_tasks; i++) {
        sorted_tasks[i] = tasks + i;
    }
    
    for (i = 0; i < num_tasks; i++) {
        min_idx = i;
        for (j = i + 1; j < num_tasks; j++) {
            if (sorted_tasks[j]->T < sorted_tasks[min_idx]->T) { 
                min_idx = j;
            }
        }
        // Swap in place.
        task_t* temp_min = sorted_tasks[min_idx];
        sorted_tasks[min_idx] = sorted_tasks[i];
        sorted_tasks[i] = temp_min;
    }
    
    // Init TCBs
    allocate_tasks(sorted_tasks, num_tasks);
    
    // Start tasks
    dispatch_nosave();
    
    return 0;
}

int event_wait(unsigned int dev_num) {
    if (dev_num >= NUM_DEVICES) return -EINVAL;
    
    // Put calling task on the device queue.
    dev_enqueue(curr_tcb, dev_num);
    
    // Sleep current task.
    dispatch_sleep();
    
    return 0;
}
