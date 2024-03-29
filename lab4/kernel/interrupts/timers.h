/**
 * @file    timers.h
 *
 * @brief   Timing-related declarations
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    18 Nov 2013
 */

// Watchdog period (In milliseconds)
#define WATCHDOG_PERIOD 200

// Tracks # of clock overflows since kernel init.
extern volatile uint32_t clock_overflows;
