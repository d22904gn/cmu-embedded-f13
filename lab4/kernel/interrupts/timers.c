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
#define OSCR_MS_ERROR 2

// Returns true if the time difference between oscr_match and oscr_val
// is less than OSCR_MS_ERROR milliseconds.
bool time_equal(uint32_t oscr_val, uint32_t oscr_match) {
    // Account for sign error.
    uint32_t time_diff = oscr_val - oscr_match;
    if (oscr_val < oscr_match) time_diff = oscr_match - oscr_val;
    
    return (get_ms(time_diff) <= OSCR_MS_ERROR);
}

/*
 * Timer interrupt handlers
 */
/**
 * @brief: Interrupt for sleep() calls.
 */
void handle_sleep() {
    uint32_t curr_oscr = reg_read(OSTMR_OSCR_ADDR);
    bool have_to_dispatch = FALSE;
    
    // Update our current sleep match to default.
    curr_sleep_match = UINT32_MAX;
    
    // Update sleepers and check if we need to wake any sleeping tasks.
    uint32_t i;
    for (i = 0; i < MAX_SLEEPERS; i++) {
        // Ignore empty entries
        if (sleepers[i].task == 0) continue;
        
        // Update sleepers
        if (sleepers[i].overflows_needed > 0
            && time_equal(curr_oscr, sleepers[i].start_oscr)) {
            // Handle overflow cases
            sleepers[i].overflows_needed--;

            // If no more overflows are needed, check if we need to
            // wake up the task soon.
            if (sleepers[i].overflows_needed == 0
                && sleepers[i].wake_match < curr_sleep_match) {
                curr_sleep_match = sleepers[i].wake_match;
            }
        } else if (sleepers[i].overflows_needed == 0
                   && time_equal(curr_oscr, sleepers[i].wake_match)) {
            // Handle waking task cases.
            
            // Update runqueue and check if we need to dispatch.
            tcb_t *wake_task = sleepers[i].task;
            runqueue_add(wake_task, wake_task->curr_prio);
            if (is_higher_prio(wake_task)) have_to_dispatch = TRUE;
            
            // Remove sleeper from the list.
            sleepers_remove(i);
        } else {
            // Handle remaining cases. (I.e. see if there are any lower
            // OSCR matches in the sleeper list.)
            if (sleepers[i].overflows_needed == 0
                && sleepers[i].wake_match < curr_sleep_match) {
                curr_sleep_match = sleepers[i].wake_match;
            } else if (sleepers[i].overflows_needed > 0
                       && sleepers[i].start_oscr < curr_sleep_match) {
                curr_sleep_match = sleepers[i].start_oscr;
            }
        }
    }
    
    // Update OSMR
    reg_write(OSTMR_OSMR_ADDR(0), curr_sleep_match);
    
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
    
    // Context switch tasks if we have to.
    if (have_to_dispatch) dispatch_save();
}

// Interrupt for time() calls.
void handle_time() {
    clock_overflows++;
    
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M1);
}

// Handle device interrupts.
void handle_devices() {    
    // Save current OSCR to reduce skew.
    uint32_t curr_oscr = reg_read(OSTMR_OSCR_ADDR);
    
    // Wake devices.
    dev_update(time());
    
    // Update our device match register.
    reg_write(OSTMR_OSMR_ADDR(2),
        curr_oscr + get_ticks(DEV_INT_PERIOD));
    
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M2);
}
