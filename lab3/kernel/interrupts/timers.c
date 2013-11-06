/**
 * @file    timers.c
 *
 * @brief   Support code for timing.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    03 Nov 2013
 */

#include "timers.h"

// Updates OSSR to indicate that a timer interrupt has been handled.
void set_timer_handled(uint32_t timer) {
    reg_set(OSTMR_OSSR_ADDR, timer);
}

// Handle timer interrupts
void handle_sleep() {
    // Update timer registers
    set_timer_handled(OSTMR_OSSR_M0);
    
    // Wake program up.
    
    // Increment num Overflows encountered
    num_overflows++;
    
    // Set Match Reg to fire next overflow again
    unsigned long curr_tick = reg_read(OSTMR_OSCR_ADDR);
    reg_write(OSTMR_OSSR_M0,curr_tick - 1);
}

void handle_time() {
    set_timer_handled(OSTMR_OSSR_M1);
    clock_overflows++;
}
