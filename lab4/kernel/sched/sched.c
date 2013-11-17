/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

// Priority level for tasks that have not been run before.
#define FIRST_RUN_PR    UINT8_MAX

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init(task_t* main_task __attribute__((unused))) {
    // ???
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void idle(void) {
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
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
void allocate_tasks(task_t** tasks, size_t num_tasks) {
    // Clear the runqueue.
    runqueue_init();
    
    // Init TCBs
	size_t i;
    for (i = 0; i < num_tasks; i++) {
        // TODO: Sanity checks
        
        // Setup prioities and locks
        system_tcb[i].native_prio = i;
        system_tcb[i].cur_prio = FIRST_RUN_PR;
        system_tcb[i].holds_lock = 0;
        system_tcb[i].sleep_queue = 0;
        
        // Setup task stack for launch_task
        system_tcb[i].context.r4 = (uint32_t) tasks[i]->lambda;
        system_tcb[i].context.r5 = (uint32_t) tasks[i]->data;
        system_tcb[i].context.r6 = (uint32_t) tasks[i]->stack_pos;
        
        // TODO: Setup kernel context?
        
        // Add to runqueue
        runqueue_add(system_tcb + i, i);
    }
    
    // Init idle TCB
    system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].cur_prio = FIRST_RUN_PR;
    system_tcb[IDLE_PRIO].holds_lock = 0;
    system_tcb[IDLE_PRIO].sleep_queue = 0;
    system_tcb[IDLE_PRIO].context.r4 = (uint32_t) idle;
    system_tcb[IDLE_PRIO].context.r5 = 0;
    system_tcb[IDLE_PRIO].context.r6 = 0;
    
    runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);
    
    // Initialize the context-switching system
    dispatch_init(&system_tcb[IDLE_PRIO]);
}

