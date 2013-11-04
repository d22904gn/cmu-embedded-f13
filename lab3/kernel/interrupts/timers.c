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

#include <exports.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include "timers.h"

#define 

// Updates OSSR to indicate that a timer interrupt has been handled.
void set_timer_handled(uint32_t int_type) {
    
}

// Handle timer interrupts
void handle_sleep() {
    // Update timer registers
    
    
    // Release timer
    timer_busy = 0;
    
    // Call the callback function.
    timer_callback();
}

void handle_time() {
    
}
