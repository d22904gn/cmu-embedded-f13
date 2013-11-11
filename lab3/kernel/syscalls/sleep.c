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
#include "../interrupts/interrupts.h"
#include "../interrupts/timers.h"

// Num milliseconds for each overflow
#define OVERFLOW_MS ((UINT_MAX / OSTMR_FREQ) * 1000)

/* 
 * Sleep algorithm:
 *   1. Split time to wait into blocks of 10 minutes (Time to overflow)
 *   2. Wait out as many 10 minute blocks as needed.
 *   3. Wait out last remaining block.
 */
void sleep(unsigned long millis) {
    // Step 1
    uint32_t overflows_needed = millis / OVERFLOW_MS;
    uint32_t remainder = get_ticks(millis % OVERFLOW_MS);
    
    // Step 2
    reg_write(OSTMR_OSMR_ADDR(0), reg_read(OSTMR_OSCR_ADDR));
    while (sleep_interrupts < overflows_needed);
    
    // Step 3
    reg_write(OSTMR_OSMR_ADDR(0), reg_read(OSTMR_OSCR_ADDR) + remainder);
    while (sleep_interrupts == overflows_needed);
    
    // Reset sleep_interrupts, since we are done.
    sleep_interrupts = 0;
}
