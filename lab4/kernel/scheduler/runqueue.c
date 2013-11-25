/** 
 * @file    runqueue.c
 * 
 * @brief   Run queue maintainence routines.
 *
 * @author  Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date    2008-11-21
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    18 Nov 2013
 */

#include <types.h>
#include <task.h>

// XXX:Debug
#include <exports.h>

static tcb_t* run_list[OS_MAX_TASKS];

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8];

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits;

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[] = {
0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init() {
    int i;
    
    for (i = 0; i < OS_MAX_TASKS / 8; i++) {
        run_bits[i] = 0;
    }
    
    group_run_bits = 0;
    
    for (i = 0; i < OS_MAX_TASKS; i++) {
        run_list[i] = 0;
    }
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb, uint8_t prio) {
    // Sanity check
	if (prio > OS_MAX_TASKS - 1 || run_list[prio]) return;
    
    // printf("\n>>>Adding task with prio %u to runqueue.\n", prio);
    
    // Add task to queue
    run_list[prio] = tcb;
    
    // Set appropriate bits in run_bits and group_run_bits
    uint8_t group = prio >> 3;
    uint8_t run_bit_offset = prio & 0x07;
    group_run_bits |= (1 << group);
    run_bits[group] |= (1 << run_bit_offset);
}

/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio) {
    // Sanity check
	if (prio > OS_MAX_TASKS - 1 || !run_list[prio]) return 0;
    
    // If we are removing the idle task, don't actually remove it.
    if (prio == IDLE_PRIO) return run_list[IDLE_PRIO];
    
    // Remove TCB from run list.
    tcb_t *temp = run_list[prio];
    run_list[prio] = 0;
    
    // Clear run queue data assoc. with TCB.
    uint8_t group = prio >> 3;
    uint8_t run_bit_offset = prio & 0x07;
    run_bits[group] &= ~(1 << run_bit_offset);
    if (run_bits[group] == 0) group_run_bits &= ~(1 << group);
    
	return temp;
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void) {
    // See algorithm in lecture slides.
    uint8_t y = prio_unmap_table[group_run_bits];
    uint8_t x = prio_unmap_table[run_bits[y]];
    
    return (y << 3) + x;
}
