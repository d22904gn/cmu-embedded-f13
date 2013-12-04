/**
 * @file    sleep_tasking.h
 *
 * @brief   Tasking support declarations for sleep() syscall.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    01 Dec 2013
 */

#ifndef SLEEP_TASKING_H
#define SLEEP_TASKING_H

#include <types.h>
#include <task.h>

// For readability
#define MAX_SLEEPERS OS_AVAIL_TASKS

// Sleeper structures
struct sleeper {
    uint32_t    start_oscr;
    uint32_t    prev_clock_oflow;
    
    uint32_t    overflows_needed;
    uint32_t    wake_match;
    tcb_t       *task;
};
typedef struct sleeper sleeper_t;

/*
 * Sleeper list declaration
 * 
 * Technically this should be a doubly linked list for max efficiency
 * during sleep interrupts. But we have no heap, so implementing this
 * would be really convoluted. Therefore the answer is fuck it and use
 * a fixed size array instead.
 */ 
extern sleeper_t sleepers[MAX_SLEEPERS];

// Tracks next OSMR match for the sleep interrupt.
extern volatile uint32_t curr_sleep_match;

// Tracks number of sleepers on the sleeper list.
extern volatile uint32_t sleeper_count;

// Sleeper list manipulation functions
void sleepers_init();
void sleepers_add(tcb_t *task,
                  uint32_t overflows_needed,
                  uint32_t remainder_ticks);
void sleepers_remove(uint32_t index);

#endif
