/**
 * @file    timer_vars.h
 *
 * @brief   Timing-related variable declarations
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    18 Nov 2013
 */

// Tracks # of clock overflows since kernel init.
extern volatile uint32_t clock_overflows;

// Tracks number of sleep interrupts encountered so far.
extern volatile uint32_t sleep_interrupts;