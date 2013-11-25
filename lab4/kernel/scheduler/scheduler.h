/**
 * @file    scheduler.h
 *
 * @brief   Scheduler related routines.
 *          Based on lab4 handout code.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#include <types.h>
#include <task.h>

// Globals
extern tcb_t *curr_tcb;         // Currently running task.

// C Routines
void allocate_tasks(task_t** tasks, uint32_t num_tasks);
void dispatch_save();
void dispatch_nosave();
void dispatch_sleep();

