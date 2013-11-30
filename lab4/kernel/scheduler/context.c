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
#include <arm/interrupt.h>
#include "runqueue.h"
#include "scheduler.h"

// Global which tracks the currently executing task.
tcb_t *curr_tcb = 0;

// ASM routines
void ctx_switch_full(sched_context_t *next_ctx,
                     sched_context_t *cur_ctx);
void ctx_switch_half(sched_context_t *next_ctx);

/**
 * @brief Context switch to the next highest priority task that is not
 * the current task while saving off the current task state.
 */
void dispatch_save() {
    INT_ATOMIC_START;
    tcb_t *next_tcb = runqueue_remove(highest_prio());
    tcb_t *old_tcb = curr_tcb;
    
    // Mark current task as runnable.
    runqueue_add(curr_tcb, curr_tcb->curr_prio);
    INT_ATOMIC_END;
    
    // Update current TCB.
    curr_tcb = next_tcb;
    
    // Context switch via ASM.
    ctx_switch_full(&(next_tcb->context), &(old_tcb->context));
}

/**
 * @brief Context switch to the highest priority task that is not the 
 * current task without saving the current task state.
 */
void dispatch_nosave() {    
    INT_ATOMIC_START;
    tcb_t *next_tcb = runqueue_remove(highest_prio());
    
    // Mark current task as runnable.
    runqueue_add(curr_tcb, curr_tcb->curr_prio);
    INT_ATOMIC_END;
    
    // Update current TCB.
    curr_tcb = next_tcb;
    
    // Context switch via ASM
    ctx_switch_half(&(next_tcb->context));
}

/**
 * @brief Context switch to the next highest priority task that is not
 * the current task while saving the current task state. Do not mark the 
 * current task as runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep() {    
    INT_ATOMIC_START;
    tcb_t *next_tcb = runqueue_remove(highest_prio());
    INT_ATOMIC_END;
    
    tcb_t *old_tcb = curr_tcb;
    
    // Update current TCB.
    curr_tcb = next_tcb;
    
    // Context switch via ASM.
    ctx_switch_full(&(next_tcb->context), &(old_tcb->context));
}
