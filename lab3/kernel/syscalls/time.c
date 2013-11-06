/**
 * @file    time.c
 *
 * @brief   Time() system call.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    06 Nov 2013
 */

#include <types.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include "../interrupts/timers.h"
#include <exports.h>

// MATH!
#define MS_PER_HOUR 3600000u

unsigned long time() {
    uint32_t curr_oscr = reg_read(OSTMR_OSCR_ADDR);
    return (clock_overflows * MS_PER_HOUR) + get_ms(curr_oscr);
}
