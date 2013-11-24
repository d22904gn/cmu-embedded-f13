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
#include "../syscalls/syscalls.h"
#include "../scheduler/devices.h"

/*
 * Globals
 */
// Tracks # of clock overflows since kernel init.
volatile uint32_t clock_overflows = 0;

// Tracks number of sleep interrupts encountered so far.
volatile uint32_t sleep_interrupts = 0;

/*
 * Timer interrupt handlers
 */

// Interrupt for sleep() calls.
void handle_sleep() {
    // Increment number of interrupts encountered
    sleep_interrupts++;
    
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
}

// Interrupt for time() calls.
void handle_time() {
    clock_overflows++;
    
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M1);
}

void handle_devices() {
    // Piggyback on our time syscall.
    unsigned long curr_time = time();
    
    // Wake devices.
    dev_update(curr_time);
    
    // Update our device match register.
    reg_write(OSTMR_OSSR_M2, get_ticks(curr_time + DEV_INT_PERIOD));
    
    // Signal interrupt handled.
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M2);
}
