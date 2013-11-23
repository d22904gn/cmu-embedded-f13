/** @file scheduler.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    18 Nov 2013
 */

#include <types.h>
#include <config.h>
#include <task.h>
#include "run_queue.h"
#include "context.h"

// Priority level for tasks that have not been run before.
#define FIRST_RUN_PR    UINT8_MAX

// Convenience macros
#define GET_KSTACK_START(task_block) (\
    sizeof(task_block) - sizeof(uint32_t) \
)

tcb_t system_tcb[OS_MAX_TASKS]; /* allocate memory for system TCBs */

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
static void idle(void) {
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
        system_tcb[i].curr_prio = FIRST_RUN_PR;
        system_tcb[i].holds_lock = 0;
        
        // Setup task stack for launch_task
        // r4 -> user entry point.
        // r5 -> single argument to the user function called.
        // r6 -> user-mode stack pointer.
        // r7 -> kernel mode stack pointer
        system_tcb[i].context.r4 = (uint32_t) tasks[i]->lambda;
        system_tcb[i].context.r5 = (uint32_t) tasks[i]->data;
        system_tcb[i].context.r6 = (uint32_t) tasks[i]->stack_pos;
        system_tcb[i].context.r7 = 
            (uint32_t) GET_KSTACK_START(system_tcb[i]);
        
        // Add to runqueue
        runqueue_add(&system_tcb[i], i);
    }
    
    // Init idle TCB
    system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].curr_prio = FIRST_RUN_PR;
    system_tcb[IDLE_PRIO].holds_lock = 0;
    system_tcb[IDLE_PRIO].context.r4 = (uint32_t) idle;
    system_tcb[IDLE_PRIO].context.r5 = 0;
    system_tcb[IDLE_PRIO].context.r6 = 0;
    system_tcb[IDLE_PRIO].context.r7 = 
            (uint32_t) GET_KSTACK_START(system_tcb[i]);
    
    runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);
}

