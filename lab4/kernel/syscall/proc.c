/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int task_create(task_t* tasks, size_t num_tasks) {
    task_t* task_array[num_tasks];
    
    // TODO: Implement priority for RMA algo.
    // Using FCFS for now.
    int i;
    for (i = 0; i < num_tasks; i++) {
        task_array[i] = tasks + i;
    }
    
    // Init TCBs
    allocate_tasks(task_array, num_tasks);
    
    // Jumpstart dev_update
    //dev_update(0);
    
    // Start highest priority task.
    //sched_init(task_array[0]);
    
    return 1; /* remove this line after adding your code */
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
    return 1; /* remove this line after adding your code */	
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
