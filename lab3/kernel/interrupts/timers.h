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

#include <exports.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>

#ifndef _TIMERS_H_
#define _TIMERS_H_

#include <inline.h>

// Tracks # of clock overflows since kernel init.
extern uint32_t clock_overflows;

// Number of overflows used for sleep
extern volatile int num_overflows;

// Timer interrupt handlers.
void handle_sleep();
void handle_time();

// Convert a OSCR reading to milliseconds
INLINE unsigned long get_ms(uint32_t counter_val)

#endif
