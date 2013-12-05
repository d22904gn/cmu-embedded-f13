/** 
 * @file    task.h
 *
 * @brief   Declares task maintainence structures.
 *
 * @author  Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date    2008-11-19
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    20 Nov 2013
 */

#ifndef _TASK_H_
#define _TASK_H_

#include <types.h>
#include <config.h>

/**
 * A task takes an arbitrary parameter and begins execution.
 * Task function are not allowed to exit or crash under any circumstance.
 */
typedef void (*task_fun_t)(void*);

struct task {
    task_fun_t    lambda;      /* The root function of this task */
    void*         data;        /* Argument to the root function */
    void*         stack_pos;   /* The starting position of the task's sp */
    unsigned long C;           /* The worst-case computation time */
    unsigned long T;           /* The task's period */
    unsigned long B;           /* Blocking term */
};
typedef struct task task_t;

/**
 * Register context for the scheduler.
 */
struct sched_context {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t sp;
    uint32_t lr;
};
typedef volatile struct sched_context sched_context_t;

/* HLP Queue notes:
 * HLP Queue implements a simple linked list. It is used for
 * accomodating tasks of the same curr_prio in the runqueue. Such a 
 * situation may occur when different tasks acquire different mutexes
 * and go to sleep.
 * 
 * Consider the following scenario:
 *   We have 3 tasks (In descending native priority): T1, T2, T3
 *   We have 3 mutexes: M1, M2, M3
 * 
 *   Each task's behaviour is:
 *     1. Acquire their corresponding mutex. (E.g. T1 -> M1, T2 -> M2)
 *     2. Do something.
 *     3. Sleep for some arbitrary period.
 *     4. Do something else.
 *     5. Call event_wait().
 * 
 *   Now assume the following order of execution:
 *     1. T1 runs. It obtains M1 and gets curr_prio = 0. Then it
 *        sleep()s for some arbitrary time.
 *     2. T2 runs. It obtains M2 and gets curr_prio = 0. Then it
 *        sleep()s for some arbitrary time.
 *     3. T3 runs. It obtains M3 and gets curr_prio = 0. Then it
 *        sleep()s for some arbitrary time.
 *     4. The idle task runs.
 *     5. T1 wakes up. Since there is no current task, it runs.
 *     6. T3 wakes up. It enters the runqueue at curr_prio = 0.
 *     7. T2 wakes up. It enters the runqueue at curr_prio = 0.
 *   
 *   Notye that when a task wakes up, it is sent into the runqueue at
 *   run_list[curr_prio]. Observe that in this case, we are inserting 2
 *   tasks (T3 then T2) with the same curr_prio into the runqueue
 *   without removing T3. This is where HLP queue comes in; it is used
 *   to accomodate T2 when it is inserted into the same position that
 *   T3 is in inside the runqueue.
 * 
 *   Note that we don't restore waking tasks to their native priority
 *   before sending them to the runqueue; doing so would mean that the 
 *   highest_prio() function is invalid since it would reflect the 
 *   native_prio of the tasks instead of their curr_prio. Hence the HLP
 *   queue is used to ensure that the tasks in the runqueue are 
 *   dispatched at their correct priority.
 */
struct tcb {
    uint8_t         native_prio;        /* The native priority of the task without escalation */
    uint8_t         curr_prio;          /* The current priority of the task after priority inheritance */
    sched_context_t context;            /* The task's serialized context -- if not running */
    bool            holds_lock;         /* True if task holds a lock */
    volatile struct tcb *hlp_queue_spot;  /* Task HLP queue. */
    /* Embed the kernel stack here -- AAPCS wants 8 byte alignment */
    uint32_t        kstack[OS_KSTACK_SIZE/sizeof(uint32_t)] 
                         __attribute__((aligned(8)));
};
typedef volatile struct tcb tcb_t;

#endif /* _TASK_H_ */
