/**
 * @file    timers.c
 *
 * @brief   Support code for handling timing interrupts.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    03 Nov 2013
 */

#include <arm/timer.h>
#include <arm/reg.h>
#include <bits/swi.h>
#include <types.h>
#include <config.h>
#include "../syscalls/syscalls.h"
#include "../syscalls/sleep_tasking.h"
#include "../scheduler/scheduler.h"

/*
 * Globals
 */
// Tracks # of clock overflows since kernel init.
volatile uint32_t clock_overflows = 0;

/*
 * Helper functions
 */
// Maximum miliseconds skew a timer can have for sleeping
// E.g. due to jitter from interrupts, etc.
#define OSCR_MS_ERROR 45

// Returns true if the time difference between oscr_match and oscr_val
// is less than OSCR_MS_ERROR milliseconds.
bool time_equal(uint32_t oscr_val, uint32_t oscr_match) {
    // Account for sign error.
    uint32_t oscr_diff = oscr_val - oscr_match;
    if (oscr_val < oscr_match) oscr_diff = oscr_match - oscr_val;
    
    return (oscr_diff <= get_ticks(OSCR_MS_ERROR));
}

/*
 * Timer interrupt handlers
 */
/**
 * @brief: Interrupt for sleep() calls.
 * 
 * See sleep() syscall for more details about sleeping algorithm.
 * 
 * Algorithm:
 *   Assumes: At each interrupt, at least one task in the sleeper list
 *            needs to either wake up or have their overflow count
 *            decremented.
 *   Steps:
 *   1. For each sleeping task in the sleeper list, check if their 
 *      overflow count needs to be decremented (Step 2a) or if they
 *      should be woken up. (Step 2b)
 *   2a. If the overflow count should be decremented, decrement the
 *       count and check if the match register needs to be updated.
 *   2b. If the task needs to be woken up, add them to the runqueue and
 *       check if we need to explicitly dispatch them. (I.e When the
 *       task has a higher prio than the current task.)
 *   3. Now search for the nearest sleeping task that needs to be
 *      serviced in the future and set the match register to trigger
 *      the sleep interrupt again at that time.
 */
void handle_sleep() {
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
    
    uint32_t curr_oscr = reg_read(OSTMR_OSCR_ADDR);
    bool have_to_dispatch = FALSE;
    
    // Update overflows and check if we need to wake any sleeping tasks.
    uint32_t i;
    for (i = 0; i < MAX_SLEEPERS; i++) {
        // Ignore empty entries
        if (sleepers[i].task == 0) continue;
        
        // Handle overflow cases
        if (sleepers[i].overflows_needed > 0
            && time_equal(curr_oscr, sleepers[i].start_oscr)) {
            
            sleepers[i].overflows_needed--;
        
        // Handle waking task cases.
        } else if (sleepers[i].overflows_needed == 0
                   && time_equal(curr_oscr, sleepers[i].wake_match)) {
            
            // Update runqueue and check if we need to dispatch.
            tcb_t *wake_task = sleepers[i].task;
            runqueue_add(wake_task, wake_task->curr_prio);
            if (is_higher_prio(wake_task)) have_to_dispatch = TRUE;
            
            // Remove sleeper from the list.
            sleepers_remove(i);
        }
    }
    
    // Now search for the next task to be serviced. Technically this can
    // be integrated in the above loop but we separate it for more
    // readability and maintainability. (This algo is convoluted enough
    // as it is.)
    curr_sleep_match = UINT32_MAX;
    
    for (i = 0; i < MAX_SLEEPERS; i++) {
        // Ignore empty entries
        if (sleepers[i].task == 0) continue;
        
        // For tasks with no overflows we check wake_match.
        if (sleepers[i].overflows_needed == 0
            && sleepers[i].wake_match < curr_sleep_match) {
            
            curr_sleep_match = sleepers[i].wake_match;
        
        // For tasks with overflows we check start_oscr.
        } else if (sleepers[i].overflows_needed > 0
                   && sleepers[i].start_oscr < curr_sleep_match) {
            
            curr_sleep_match = sleepers[i].start_oscr;
        }
    }
    
    // Update OSMR
    reg_write(OSTMR_OSMR_ADDR(0), curr_sleep_match);
    
    // Context switch tasks if we have to.
    if (have_to_dispatch) dispatch_save();
}

// Interrupt for time() calls.
void handle_time() {
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M1);
    
    clock_overflows++;
}

// Handle device interrupts.
void handle_devices() {
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M2);
    
    uint32_t curr_oscr = reg_read(OSTMR_OSCR_ADDR);
    
    // Update our device match register.
    reg_write(OSTMR_OSMR_ADDR(2),
        curr_oscr + get_ticks(DEV_INT_PERIOD));
    
    // Wake devices.
    dev_update(time());
}
