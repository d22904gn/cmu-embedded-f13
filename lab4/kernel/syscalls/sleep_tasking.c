/**
 * @file    sleep_tasking.c
 *
 * @brief   Tasking support facade for sleep() syscall.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    01 Dec 2013
 */

#include <types.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>
#include <task.h>
#include "../scheduler/scheduler.h"
#include "../interrupts/timers.h"
#include "sleep_tasking.h"

// Sleeper list declaration
sleeper_t sleepers[MAX_SLEEPERS];

// Tracks next OSMR match for the sleep interrupt.
volatile uint32_t curr_sleep_match = UINT32_MAX;

// Tracks number of sleepers on the sleeper list.
volatile uint32_t sleeper_count = 0;

// Initialization / Reset
void sleepers_init() {
    uint32_t i;
    
    // Reset the sleepers list.
    for (i = 0; i < MAX_SLEEPERS; i++) {
        sleepers[i].start_oscr = 0;
        sleepers[i].overflows_needed = 0;
        sleepers[i].wake_match = 0;
        sleepers[i].task = (tcb_t*) 0;
    }
    
    // Reset sleeper count
    sleeper_count = 0;
    
    // Reset the match register.
    curr_sleep_match = UINT32_MAX;
    reg_write(OSTMR_OSMR_ADDR(0), UINT32_MAX);
    
    // Reset watchdog for taking care of jitter.
    reg_write(OSTMR_OSMR_ADDR(3),
        reg_read(OSTMR_OSCR_ADDR) + get_ticks(WATCHDOG_PERIOD));
}

// Add a sleeper to the list.
void sleepers_add(tcb_t *task,
                  uint32_t overflows_needed,
                  uint32_t remainder_ticks) {
    // Record down OSCR value at which the sleeper was added.
    uint32_t start_oscr = reg_read(OSTMR_OSCR_ADDR);
    sleepers[task->native_prio].start_oscr = start_oscr;
    
    // Other vars
    sleepers[task->native_prio].prev_clock_oflow = clock_overflows;
    sleepers[task->native_prio].overflows_needed = overflows_needed;
    sleepers[task->native_prio].task = task;
    
    // Calculate the OSCR value at which this task will wake up.
    uint32_t wake_match = 
        sleepers[task->native_prio].start_oscr + remainder_ticks;
    sleepers[task->native_prio].wake_match = wake_match;
    
    // No funky business with other sleeping stuff waking up while we're
    // modifying shared sleeping data structures!
    INT_ATOMIC_START;
    sleeper_count++;
    
    /* Check if we need to update OSMR. (I.e. when this task sleeps less
     * than other sleeping tasks.) */
    if (overflows_needed == 0 && wake_match < curr_sleep_match) {
        // Handle scenario where we don't need to count overflows.
        // Sufficient to check if this task will wake up earlier than
        // other sleeping tasks.
        curr_sleep_match = wake_match;
        reg_write(OSTMR_OSMR_ADDR(0), wake_match);
    } else if (overflows_needed > 0 && start_oscr < curr_sleep_match) {
        // Handle scenario where we need to count overflows.
        // This check is for the situation where we don't have any
        // other sleeping tasks. So we force an OSMR update.
        curr_sleep_match = start_oscr;
        reg_write(OSTMR_OSMR_ADDR(0), start_oscr);
    }
    INT_ATOMIC_END;
}

// Remove a sleeper from the sleeper list. Only called from within
// an interrupt.
void sleepers_remove(uint32_t index) {
    sleepers[index].start_oscr = 0;
    sleepers[index].prev_clock_oflow = 0;
    sleepers[index].overflows_needed = 0;
    sleepers[index].wake_match = 0;
    sleepers[index].task = (tcb_t*) 0;

    sleeper_count--;
}
