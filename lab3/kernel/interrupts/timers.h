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

#include <arm/timer.h>
#include <inline.h>

// Tracks # of clock overflows since kernel init.
extern uint32_t clock_overflows;

// Number of overflows used for sleep
extern volatile uint32_t num_overflows;

// Timer interrupt handlers.
void handle_sleep();
void handle_time();

// Convert a OSCR reading to milliseconds
INLINE unsigned long get_ms(uint32_t counter_val) {
    return (counter_val / OSTMR_FREQ) * 1000;
}

#endif
