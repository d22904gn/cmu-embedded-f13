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

#include <inline.h>
#include <types.h>
#include <arm/timer.h>
#include <arm/reg.h>

// Milliseconds before OSCR will overflow.
#define MS_PER_OVERFLOW ((uint32_t) ((UINT_MAX / OSTMR_FREQ) * 1000u))

// Tracks # of clock overflows since kernel init.
extern volatile uint32_t clock_overflows;

// Tracks number of sleep interrupts encountered so far.
extern volatile uint32_t sleep_interrupts;

// Convert a OSCR reading to milliseconds
INLINE unsigned long get_ms(uint32_t counter_val) {
    return counter_val / (OSTMR_FREQ / 1000);
}

// Convert milliseconds to num of OSCR ticks needed.
// Assumes millis <= <counter size> / <clock freq>
INLINE uint32_t get_ticks(uint32_t millis) {
    return (OSTMR_FREQ / 1000) * millis;
}

/* Actual time() syscall */
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
    uint32_t overflows_needed = millis / MS_PER_OVERFLOW;
    uint32_t remainder = get_ticks(millis % MS_PER_OVERFLOW);
    
    // Step 2
    reg_write(OSTMR_OSMR_ADDR(0), reg_read(OSTMR_OSCR_ADDR));
    while (sleep_interrupts < overflows_needed);
    
    // Step 3
    reg_write(OSTMR_OSMR_ADDR(0), reg_read(OSTMR_OSCR_ADDR) + remainder);
    while (sleep_interrupts == overflows_needed);
    
    // Reset sleep_interrupts, since we are done.
    sleep_interrupts = 0;
}
