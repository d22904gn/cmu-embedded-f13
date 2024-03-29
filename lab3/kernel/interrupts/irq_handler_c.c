/**
 * @file    irq_handler_c.c
 *
 * @brief   Custom interrupt handler in C
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    03 Nov 2013
 */

#include <inline.h>
#include <arm/reg.h>
#include <arm/interrupt.h>

// Defines masks to check which interrupt was asserted.
#define SLEEP_TIMER (1 << INT_OSTMR_0)
#define TIME_TIMER  (1 << INT_OSTMR_1)

// Device handlers
void handle_time();
void handle_sleep();

void irq_handler_c() {
    // Check if timer generated the interrupt
    int int_source = reg_read(INT_ICMR_ADDR) & reg_read(INT_ICPR_ADDR);
    if (!(int_source & (SLEEP_TIMER | TIME_TIMER))) return;
    
    /*
     * Unlikely that both will occur at the same time.
     */
    // Check for time() interrupt.
    if (int_source & TIME_TIMER) handle_time();
    
    // Check for sleep() interrupt.
    if (int_source & SLEEP_TIMER) handle_sleep();
}
