/**
 * @file    timers.h
 *
 * @brief   Support code declarations for timing-related stuff.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    03 Nov 2013
 */

#ifndef TIMERS_H
#define TIMERS_H

#include <inline.h>
#include <types.h>
#include <arm/timer.h>

// Tracks # of clock overflows since kernel init.
extern volatile uint32_t clock_overflows;

// Tracks number of sleep interrupts encountered so far.
extern volatile uint32_t sleep_interrupts;

// Convert a OSCR reading to milliseconds
INLINE unsigned long get_ms(uint32_t counter_val) {
    return counter_val / (OSTMR_FREQ / 1000);
}

// Convert milliseconds to num of OSCR ticks needed.
// Assumes millis <= <counter size> / <clock freq>
INLINE uint32_t get_ticks(uint32_t millis) {
    return (OSTMR_FREQ / 1000) * millis;
}

#endif
