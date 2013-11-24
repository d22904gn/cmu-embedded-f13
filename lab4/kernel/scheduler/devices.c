/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <config.h>
#include <types.h>
#include <task.h>
#include "scheduler.h"
#include "devices.h"
#include "runqueue.h"
#include "tcbqueue.h"

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init() {
    int i;
    
    for (i = 0; i < NUM_DEVICES; i++) {
        tcbqueue_init(&(devices[i].sleep_queue));
        devices[i].next_match = dev_freq[i];
    }
}

/**
 * @brief Adds a task to the device sleep queue.
 */
void dev_enqueue(tcb_t *task, unsigned int dev_num) {
    // Sanity check
    if (dev_num >= NUM_DEVICES ||
        tcbqueue_full(&(devices[dev_num].sleep_queue))) return;

    tcbqueue_enqueue(&(devices[dev_num].sleep_queue), task);
}

/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis) {
	uint32_t i;
    
    for (i = 0; i < NUM_DEVICES; i++) {
        if (devices[i].next_match <= millis) {
            // Wake TCBs
            while (devices[i].sleep_queue.size != 0) {
                tcb_t *next_tcb = 
                    tcbqueue_dequeue(&(devices[i].sleep_queue));
                
                runqueue_add(next_tcb, next_tcb->native_prio);
            }
            
            // Update next_match
            devices[i].next_match += dev_freq[i];
        }
    }
    
    // Switch into the new TCBs. No need to save since all tasks have
    // been sleeping.
    dispatch_nosave();
}

