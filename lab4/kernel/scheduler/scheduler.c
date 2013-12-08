/**
 * @file    scheduler.c
 * 
 * @brief   Top level implementation of the scheduler.
 *
 * @author  Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date    2008-11-20
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    20 Nov 2013
 */

#include <types.h>
#include <config.h>
#include <task.h>
#include <arm/interrupt.h>
#include <arm/physmem.h>
#include "scheduler.h"
#include "../utils/math.h"
#include "../syscalls/syscalls.h"
#include "../syscalls/sleep_tasking.h"

// Macro to calculate the start of the kernel stack for each TCB.
#define GET_KSTACK_START(task_block) ( \
    ((char*) task_block.kstack) + OS_KSTACK_SIZE \
)

tcb_t system_tcb[OS_MAX_TASKS]; /* allocate memory for system TCBs */

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
static void idle() {
    while(1);
}

/**
 * @brief Initializes the idle task.
 */
void idle_init() {
    // Init idle TCB.
    system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].curr_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].locks_held = 0;
    system_tcb[IDLE_PRIO].fifo_queue_spot = NULL;
    
    system_tcb[IDLE_PRIO].context.r4 = (uint32_t) idle;
    system_tcb[IDLE_PRIO].context.r5 = 0;
    system_tcb[IDLE_PRIO].context.r6 = 0;
    system_tcb[IDLE_PRIO].context.sp = 
            (uint32_t) GET_KSTACK_START(system_tcb[IDLE_PRIO]);
    system_tcb[IDLE_PRIO].context.lr = 0;
    
    // Init the current TCB to the idle TCB
    curr_tcb = &system_tcb[IDLE_PRIO];
}

// UB Test lookup table.
static const double u[OS_AVAIL_TASKS + 1] = {
    1.00000, 1.00000, 0.82842, 0.77976, 0.75682, 0.74349, 0.73477,
    0.72862, 0.72406, 0.72053, 0.71773, 0.71545, 0.71355, 0.71195,
    0.71059, 0.70941, 0.70838, 0.70747, 0.70666, 0.70594, 0.70529,
    0.70471, 0.70418, 0.70369, 0.70325, 0.70284, 0.70246, 0.70212,
    0.70179, 0.70149, 0.70121, 0.70095, 0.70070, 0.70047, 0.70026,
    0.70005, 0.69986, 0.69968, 0.69950, 0.69934, 0.69918, 0.69903,
    0.69889, 0.69876, 0.69863, 0.69851, 0.69839, 0.69828, 0.69817,
    0.69807, 0.69797, 0.69787, 0.69778, 0.69769, 0.69761, 0.69753,
    0.69745, 0.69737, 0.69730, 0.69723, 0.69716, 0.69710, 0.69703,
    0.69697
};

/**
 * @brief Perform UB Test and reorder the task list. Assumes no interrupts.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return FALSE  The test failed.
 * @return TRUE   Test succeeded.  The tasks are now in order.
 */
bool assign_schedule(task_t** tasks, uint32_t num_tasks) {
    // Initial sanity checks
    if (num_tasks > OS_AVAIL_TASKS) return FALSE;
    
    // Sort priorities via selection sort
    uint32_t i, j, min_idx;
    for (i = 0; i < num_tasks; i++) {
        min_idx = i;
        for (j = i + 1; j < num_tasks; j++) {
            if (tasks[j]->T < tasks[min_idx]->T) { 
                min_idx = j;
            }
        }
        // Swap in place.
        task_t* temp_min = tasks[min_idx];
        tasks[min_idx] = tasks[i];
        tasks[i] = temp_min;
    }
    
    // Perform UB test
    for (i = 0; i < num_tasks; i++) {
        double curr_u = 0.0;
        
        for (j = 0; j <= i; j++) {
            curr_u += (tasks[j]->C / tasks[j]->T);
        }
        
        // Blocking term
        curr_u += (tasks[i]->B / tasks[i]->T);
        
        if (curr_u >= u[i]) return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads. Assumes no interrupts.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
bool allocate_tasks(task_t **tasks, uint32_t num_tasks) {
    // Initial sanity checks
    if (num_tasks > OS_AVAIL_TASKS) return FALSE;
    
    // Re-init scheduler system
    runqueue_init();
    sleepers_init();
    mutex_init();
    idle_init();
    dev_init(time());
    
    /*
     * Init user TCBs
     */
    uint32_t i;
    uint32_t prio_idx = 0;
    
    for (i = 0; i < num_tasks; i++) {
        // Check for invalid tasks.
        if (!in_userspace((uint32_t) tasks[i]->lambda) ||
            !in_userspace((uint32_t) tasks[i]->stack_pos)||
            !multiple_of_8((uint32_t) tasks[i]->stack_pos)) {
            
            INT_ATOMIC_END;
            return FALSE;
        }
        
        // Setup prioties and locks
        system_tcb[prio_idx].native_prio = prio_idx;
        system_tcb[prio_idx].curr_prio = prio_idx;
        system_tcb[prio_idx].locks_held = 0;
        system_tcb[prio_idx].fifo_queue_spot = NULL;
        
        // Setup task stack for launch_task
        // r4 -> user entry point.
        // r5 -> single argument to the user function called.
        // r6 -> user-mode stack pointer.
        // sp -> kernel mode stack pointer
        // lr -> 0 for a new task.
        system_tcb[prio_idx].context.r4 = (uint32_t) tasks[i]->lambda;
        system_tcb[prio_idx].context.r5 = (uint32_t) tasks[i]->data;
        system_tcb[prio_idx].context.r6 =
            (uint32_t) tasks[i]->stack_pos;
        system_tcb[prio_idx].context.sp = 
            (uint32_t) GET_KSTACK_START(system_tcb[prio_idx]);
        system_tcb[prio_idx].context.lr = 0;
        
        // Add to runqueue
        runqueue_add(&system_tcb[prio_idx], prio_idx);
        prio_idx++;
    }
    
    return TRUE;
}

/**
 * Returns TRUE if the argument task has a higher priority than the
 * currently running task. Assumes no interrupts.
 */
bool is_higher_prio(tcb_t* task) {
    return (task->curr_prio < curr_tcb->curr_prio);
}
