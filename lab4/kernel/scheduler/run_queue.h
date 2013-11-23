/**
 * @file    run_queue.h
 *
 * @brief   Run-queue routine prototypes
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#include <types.h>
#include <task.h>

void runqueue_init(void);
void runqueue_add(tcb_t* tcb, uint8_t prio);
tcb_t* runqueue_remove(uint8_t prio);
uint8_t highest_prio(void);
