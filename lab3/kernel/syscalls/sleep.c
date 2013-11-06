/**
 * @file    sleep.c
 *
 * @brief   Sleep() system call.
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

// Num milliseconds for each overflow
// Verdex board 3.25 MHz with 32 bit unsigned int
#define OVERFLOW_MS ((UINT_MAX / OSTMR_FREQ) * 1000)

/* Sleep algorithm:
 *   1. Split time to wait into blocks of 10 minutes (Time to overflow)
 *   2. Wait out as many 10 minute blocks as needed.
 *   3. Wait out last remaining block.
 */
void sleep(unsigned long millis) {
    uint32_t num_overflows_needed = millis / OVERFLOW_MS;
    unsigned long remainder = millis % OVERFLOW_MS;
    unsigned long curr_ticks = reg_read(OSTMR_OSCR_ADDR);
    
    // Configure interrupts to occur in blocks of 10 minutes if needed.
    reg_write(OSTMR_OSSR_M0, (uint32_t) curr_ticks);
    
    // Spin until num overflows reached
    while (num_overflows < num_overflows_needed);
    
    // Set IRQ to fire in remainder time
    curr_ticks = reg_read(OSTMR_OSCR_ADDR);
    reg_write(OSTMR_OSSR_M0, (uint32_t) (curr_ticks + remainder));
    
    // Spin until remainder time finished
    while (num_overflows == num_overflows_needed);
}
