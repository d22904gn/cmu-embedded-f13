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
        run_list[i] = NULL;
    }
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.
 * The only requirement is that the run queue for that priority is
 * empty. This function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb, uint8_t prio) {
    // Sanity check.
    if (prio > OS_MAX_TASKS - 1) return;
    
    // Add task to queue. If another task of the same priority already
    // exists in the runqueue, we add it to its HLP queue.
    if (run_list[prio] != NULL) {
        // Loop through the chain until we find an empty spot.
        tcb_t *queue = run_list[prio];
        while (queue->hlp_queue_spot != NULL)
            queue = queue->hlp_queue_spot;
        
        queue->hlp_queue_spot = tcb;
    } else {
        run_list[prio] = tcb;
    }
    
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
    if (prio > OS_MAX_TASKS - 1 || !run_list[prio]) return NULL;
    
    // Remove TCB from run list.
    tcb_t *item = run_list[prio];
    
    // If TCB has a non-empty HLP queue, push the next TCB in the queue
    // up. Else zero out the runqueue position for the given priority.
    if (item->hlp_queue_spot != NULL) {
        run_list[prio] = item->hlp_queue_spot;
        item->hlp_queue_spot = NULL;
    } else {
        run_list[prio] = NULL;
        
        // Clear run queue data associated with TCB.
        uint8_t group = prio >> 3;
        uint8_t run_bit_offset = prio & 0x07;
        run_bits[group] &= ~(1 << run_bit_offset);
        if (run_bits[group] == 0) group_run_bits &= ~(1 << group);
    }
    
    return item;
}

/**
 * @brief This function examines the run bits and the run queue and
 * returns the priority of the runnable task with the highest priority
 * (lower number).
 */
uint8_t highest_prio(void) {
    // See algorithm in lecture slides.
    uint8_t y = prio_unmap_table[group_run_bits];
    uint8_t x = prio_unmap_table[run_bits[y]];
    
    return (y << 3) + x;
}
