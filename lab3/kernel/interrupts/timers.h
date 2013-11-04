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

#ifndef _TIMERS_H_
#define _TIMERS_H_

// Tracks # of clock overflows since kernel init.
uint32_t clock_overflows = 0;

// Asserted if sleep timer is currently being used.
int sleep_timer_busy = 0;

// Timer interrupt handlers.
void handle_sleep();
void handle_time();

#endif
