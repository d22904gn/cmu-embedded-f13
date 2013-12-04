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

#include <arm/reg.h>
#include <arm/interrupt.h>

// Defines masks to check which interrupt was asserted.
#define SLEEP_TIMER     (1 << INT_OSTMR_0)
#define TIME_TIMER      (1 << INT_OSTMR_1)
#define DEVICES_TIMER   (1 << INT_OSTMR_2)
#define WATCHDOG        (1 << INT_OSTMR_3)

// Specific interrupt handlers
void handle_time();
void handle_sleep();
void handle_devices();
void watchdog_woof();

void irq_handler_c() {
    int int_source = reg_read(INT_ICMR_ADDR) & reg_read(INT_ICPR_ADDR);
    
    // Process sleep() interrupt.
    if (int_source & SLEEP_TIMER) handle_sleep();
    
    // Process device interrupt.
    if (int_source & DEVICES_TIMER) handle_devices();
    
    // Process time() interrupt.
    if (int_source & TIME_TIMER) handle_time();
    
    // Process watchdog interrupt
    if (int_source & WATCHDOG) watchdog_woof();
}
