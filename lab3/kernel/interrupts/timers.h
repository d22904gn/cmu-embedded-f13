/**
 * @file    timers.h
 *
 * @brief   Support code defintions for timing-related stuff.
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

// Number of overflows used for sleep
extern volatile uint32_t num_overflows;

// Convert a OSCR reading to milliseconds
INLINE unsigned long get_ms(uint32_t counter_val) {
    // Watch for overflow
    return (((uint64_t) counter_val) * 1000) / OSTMR_FREQ;
}

// Convert milliseconds to num of OSCR ticks needed.
// Assumes millis <= <counter size> / <clock freq>
INLINE uint32_t get_ticks(uint32_t millis) {
    return (OSTMR_FREQ * ((uint64_t) millis)) / 1000;
}

#endif
