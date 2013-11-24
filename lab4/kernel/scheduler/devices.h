/**
 * @file    devices.h
 *
 * @brief   Virtual device structs and definitions.
 *          Based on lab4 handout code.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <config.h>
#include <task.h>
#include "tcbqueue.h"

#define NUM_DEVICES  4

// Device struct definition.
struct dev {
    unsigned long next_match;
	tcb_queue_t   sleep_queue;
};
typedef struct dev dev_t;

// Number of millis between each device interrupt
#define DEV_INT_PERIOD 50

void dev_init();
void dev_enqueue(tcb_t *callee, unsigned int dev_num);
void dev_update(unsigned long num_millis);
#endif /* DEVICE_H */
