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

#include <arm/timer.h>
#include <arm/reg.h>
#include "timers.h"

/*
 * Handle timer interrupts
 */

// Interrupt for sleep() calls.
void handle_sleep() {
    // Increment number of overflows encountered
    num_overflows++;
    
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
}

// Interrupt for time() calls.
void handle_time() {
    clock_overflows++;
    
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M1);
}
