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
#include <arm/interrupt.h>
#include "scheduler.h"

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 * Note that next match is set relative to millis.
 */
void dev_init(unsigned long millis) {
    int i;
    
    for (i = 0; i < NUM_DEVICES; i++) {
        tcbqueue_init(&(devices[i].sleep_queue));
        devices[i].next_match = millis + dev_freq[i];
    }
}

/**
 * @brief Adds a task to the device sleep queue.
 */
void dev_enqueue(tcb_t *task_ptr, unsigned int dev_num) {
    // Sanity check
    if (dev_num >= NUM_DEVICES ||
        tcbqueue_full(&(devices[dev_num].sleep_queue))) return;
    
    // No naughty business with the queue when we're adding to it!
    INT_ATOMIC_START;
    tcbqueue_enqueue(&(devices[dev_num].sleep_queue), task_ptr);
    INT_ATOMIC_END;
}

/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis) {
    /* Note: Only called by interrupts so will not be interrupted.
     * Also: Only function called by a device interrupt. So any data
     * structure that this mangles will result in a race condition.
     * 
     * Structures mangled:
     * 1. device TCB sleep queue
     * 2. runqueue
     */
    uint32_t i;
    bool need_to_switch = FALSE;
    
    for (i = 0; i < NUM_DEVICES; i++) {
        if (devices[i].next_match <= millis) {
            // Wake tasks
            while (devices[i].sleep_queue.size != 0) {
                tcb_t *next_tcb = 
                    tcbqueue_dequeue(&(devices[i].sleep_queue));
                
                /* Check if we need to context switch after this
                 * interrupt. */
                if (is_higher_prio(next_tcb)) need_to_switch = TRUE;
                
                runqueue_add(next_tcb, next_tcb->native_prio);
            }
            
            // Update next_match
            devices[i].next_match += dev_freq[i];
        }
    }
    
    // Switch into the new TCBs if needed.
    if (need_to_switch) dispatch_save();
}
