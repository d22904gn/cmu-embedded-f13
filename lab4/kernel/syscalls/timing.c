/**
 * @file    timing.c
 *
 * @brief   Timing related system calls.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    20 Nov 2013
 */

#include <types.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include "../scheduler/scheduler.h"
#include "../interrupts/timer_vars.h"
#include "sleep_tasking.h"

/**
 * @brief Get time from kernel start.
 **/
unsigned long time() {
    return (clock_overflows * MS_PER_OVERFLOW) +
           get_ms(reg_read(OSTMR_OSCR_ADDR));
}

/**
 * @brief Suspend task for specified milliseconds
 * 
 * Sleep algorithm:
 *   1. Split time to wait into blocks of 20 minutes (Time to overflow)
 *   2. Wait out as many 20 minute blocks as needed.
 *   3. Wait out last remaining block.
 */
void sleep(unsigned long millis) {
    // Calculate values for OSMR.
    uint32_t overflows_needed = millis / MS_PER_OVERFLOW;
    uint32_t remainder_ticks = get_ticks(millis % MS_PER_OVERFLOW);
    
    // Add current task to the sleeping list and send it to sleep.
    sleepers_add(curr_tcb, overflows_needed, remainder_ticks);
    dispatch_sleep();
}
