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
#include "runqueue.h"
#include "devices.h"
#include "mutex.h"
#include "../utils/math.h"
#include "../syscalls/syscalls.h"

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
void allocate_tasks(task_t **tasks, uint32_t num_tasks) {
    // Initial sanity checks
    if (num_tasks > OS_AVAIL_TASKS) return;
    
    // No naughty business with the runqueue when we're adding to it!
    INT_ATOMIC_START;
    
    // Re-init scheduler
    runqueue_init();
    mutex_init();
    dev_init(time());
    
    // Init TCBs
    uint32_t i;
    uint32_t prio_idx = 0;
    
    for (i = 0; i < num_tasks; i++) {
        // Skip invalid tasks.
        if (!in_userspace((uint32_t) tasks[i]->lambda) ||
            !in_userspace((uint32_t) tasks[i]->stack_pos)||
            !multiple_of_8((uint32_t) tasks[i]->stack_pos))
            continue;
        
        // Setup prioities and locks
        system_tcb[prio_idx].native_prio = prio_idx;
        system_tcb[prio_idx].curr_prio = prio_idx;
        system_tcb[prio_idx].holds_lock = FALSE;
        system_tcb[prio_idx].prio_src = 0;
        
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
    
    // Init idle TCB
    system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].curr_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].holds_lock = FALSE;
    system_tcb[IDLE_PRIO].prio_src = 0;
    
    system_tcb[IDLE_PRIO].context.r4 = (uint32_t) idle;
    system_tcb[IDLE_PRIO].context.r5 = 0;
    system_tcb[IDLE_PRIO].context.r6 = 0;
    system_tcb[IDLE_PRIO].context.sp = 
            (uint32_t) GET_KSTACK_START(system_tcb[IDLE_PRIO]);
    system_tcb[IDLE_PRIO].context.lr = 0;
    
    runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);
    
    // Init the current TCB to the idle TCB
    curr_tcb = &system_tcb[IDLE_PRIO];
    
    INT_ATOMIC_END;
}

/**
 * Returns TRUE if the argument task has a higher priority than the
 * currently running task.
 */
bool is_higher_prio(tcb_t* task) {
    return (task->curr_prio < curr_tcb->curr_prio);
}
