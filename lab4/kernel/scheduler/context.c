/** @file context.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#include <types.h>
#include <task.h>
#include "runqueue.h"
#include "scheduler.h"

// Global which tracks the currently executing task.
tcb_t *curr_tcb = 0;

// ASM routines
void ctx_switch_full(sched_context_t *next_ctx,
                     sched_context_t *cur_ctx,
                     bool launch_task);
void ctx_switch_half(sched_context_t *next_ctx, bool launch_task);

/**
 * @brief Context switch to the highest priority task that is not the 
 * current task while saving off the current task state.
 */
void dispatch_save() {
    // Check that we have a different task.
    uint8_t next_prio = highest_prio();
    if (curr_tcb->curr_prio == next_prio) return;
    
	tcb_t *next_tcb = runqueue_remove(next_prio);
    tcb_t *old_tcb = curr_tcb;
    
    // Mark current task as runnable.
    runqueue_add(curr_tcb, curr_tcb->curr_prio);
    
    // Update current TCB.
    curr_tcb = next_tcb;
    
    // Context switch via ASM.
    bool launch_task = (next_tcb->curr_prio == SLEEP_PRIO);
    ctx_switch_full(&(next_tcb->context), &(old_tcb->context),
                    launch_task);
}

/**
 * @brief Context switch to the highest priority task that is not the 
 * current task without saving the current task state.
 */
void dispatch_nosave() {
    // Check that we have a different task.
    uint8_t next_prio = highest_prio();
    if (curr_tcb->curr_prio == next_prio) return;
    
    tcb_t *next_tcb = runqueue_remove(next_prio);
    
    // Mark current task as runnable.
    runqueue_add(curr_tcb, curr_tcb->curr_prio);
    
    // Update current TCB.
    curr_tcb = next_tcb;
    
    // Context switch via ASM.
    bool launch_task = (next_tcb->curr_prio == SLEEP_PRIO);
    ctx_switch_half(&(next_tcb->context), launch_task);
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark it runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep() {
    // Check that we have a different task.
    uint8_t next_prio = highest_prio();
    if (curr_tcb->curr_prio == next_prio) return;
    
    tcb_t *next_tcb = runqueue_remove(next_prio);
    
    // Update current TCB.
    curr_tcb = next_tcb;
    
    // Context switch via ASM.
    bool launch_task = (next_tcb->curr_prio == SLEEP_PRIO);
    ctx_switch_half(&(next_tcb->context), launch_task);
}
