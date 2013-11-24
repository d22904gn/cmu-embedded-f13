/**
 * @file    tcbqueue.c
 *
 * @brief   TCB queue related declarations
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#ifndef TCBQUEUE_H
#define TCBQUEUE_H

#include <config.h>
#include <types.h>
#include <task.h>

// For readability.
#define MAX_TCBQ_SIZE OS_AVAIL_TASKS

// Queue struct
struct tcb_queue {
    uint32_t    size;
	uint32_t    head;
    uint32_t    tail;
    tcb_t*      queue[MAX_TCBQ_SIZE];
};
typedef struct tcb_queue tcb_queue_t;

// Queue manipulation functions
void tcbqueue_init(tcb_queue_t *queue);
void tcbqueue_enqueue(tcb_queue_t *queue, tcb_t *item);
tcb_t* tcbqueue_dequeue(tcb_queue_t *queue);
bool tcbqueue_full(tcb_queue_t *queue);

#endif
