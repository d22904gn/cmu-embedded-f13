/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    14 Nov 2013
 */

#include <types.h>
#include <config.h>
#include <arm/reg.h>
#include <syscall.h>
#include "time.h"

unsigned long time_syscall(void) {
    return (clock_overflows * MS_PER_OVERFLOW) +
           get_ms(reg_read(OSTMR_OSCR_ADDR));
}

/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 * Sleep algorithm:
 *   1. Split time to wait into blocks of 10 minutes (Time to overflow)
 *   2. Wait out as many 10 minute blocks as needed.
 *   3. Wait out last remaining block.
 */
void sleep_syscall(unsigned long millis  __attribute__((unused))) {
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
