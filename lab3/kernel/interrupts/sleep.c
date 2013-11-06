
#include "sleep.h"

// Num milliseconds for each overflow
// Verdex board 3.25 MHz with 32 bit unsigned int
#define OVERFLOW_MS (UINT_MAX/ 3250000) * 1000

volatile int num_overflows;

// Suspends task for time in milliseconds
void sleep(unsigned long millis) {
    int num_overflows_needed = millis / OVERFLOW_MS;
    unsigned long remainder = millis % OVERFLOW_MS;
    num_overflows = 0;
    
    // Begin wait periods
    // FIX: OSCR reg
    unsigned long curr_ticks = reg_read(OSTMR_OSCR_ADDR);
    
    // Set IRQ to fire every
    reg_write(OSTMR_OSSR_M0,(uint32_t) curr_ticks -1);
    
    // Loop until num overflows reached
    while (num_overflows < num_overflows_needed) {
    }
    
    // Set IRQ to fire in remainder time
    reg_write(OSTMR_OSSR_M0,(uint32_t) curr_ticks +remainder);
    
    // Keep looping until remainder time finished
    while (num_overflows == num_overflows_needed) {
    }
    
    return;
}
