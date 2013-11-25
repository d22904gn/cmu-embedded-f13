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
    task_fun_t    lambda;      /**< The root function of this task */
    void*         data;        /**< Argument to the root function */
    void*         stack_pos;   /**< The starting position of the task's sp */
    unsigned long C;           /**< The worst-case computation time */
    unsigned long T;           /**< The task's period */
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


struct tcb {
    uint8_t         native_prio;        /** The native priority of the task without escalation */
    uint8_t         curr_prio;          /** The current priority of the task after priority inheritance */
    sched_context_t context;            /** The task's serialized context -- if not running */
    bool            holds_lock;         /** TRUE if the task is currently owning a lock */
    volatile struct tcb* prio_src;      /** Task that this TCB inherits priority from **/
    /** Embed the kernel stack here -- AAPCS wants 8 byte alignment */
    uint32_t        kstack[OS_KSTACK_SIZE/sizeof(uint32_t)] 
                         __attribute__((aligned(8)));
};
typedef volatile struct tcb tcb_t;

#endif /* _TASK_H_ */
