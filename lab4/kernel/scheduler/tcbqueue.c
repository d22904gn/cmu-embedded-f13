/**
 * @file    tcbqueue.c
 *
 * @brief   TCB queue implementation. This is a FIFO queue for TCBs.
 *          This can be used for implementing stuff like sleep queues,
 *          etc.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#include <config.h>
#include <types.h>
#include <task.h>
#include "tcbqueue.h"
#include "../utils/math.h"

void tcbqueue_init(tcb_queue_t *queue) {
    queue->size = 0;
    queue->head = 0;
    queue->tail = 0;
    
    int i;
    for (i = 0; i < MAX_TCBQ_SIZE; i++) {
        queue->queue[i] = 0;
    }
}

bool tcbqueue_full(tcb_queue_t *queue) {
    return queue->size == MAX_TCBQ_SIZE;
}

void tcbqueue_enqueue(tcb_queue_t *queue, tcb_t *item) {
    // Sanity check
    if (tcbqueue_full(queue)) return;
    
    queue->queue[queue->tail] = item;
    queue->tail = fast_modulo(queue->tail + 1, MAX_TCBQ_SIZE);
    queue->size++;
}

tcb_t* tcbqueue_dequeue(tcb_queue_t *queue) {
    // Sanity check
    if (queue->size == 0) return 0;
    
    tcb_t *item = queue->queue[queue->head];
    queue->head = fast_modulo(queue->head + 1, MAX_TCBQ_SIZE);
    queue->size--;
    
    return item;
}

tcb_t* tcbqueue_poll(tcb_queue_t *queue) {
    // Sanity check
    if (queue->size == 0) return 0;
    
    return queue->queue[queue->head];
}
