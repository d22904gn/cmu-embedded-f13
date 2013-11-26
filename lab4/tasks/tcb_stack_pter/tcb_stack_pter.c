/** @file tcb_stack_pter.c
 *
 * @brief Creates invalid stack pointer for tcb,
 * '@' task won't executes
 * '<' task won't execute:
 * Idle task runs
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-30
 */
#include <stdio.h>
#include <task.h>
#include <unistd.h>

int mutex = 0;
int fun1_iter = 0;
int fun2_iter = 0;

void panic(const char* str)
{
	puts(str);
	while(1);
}

void fun1(void* str) {
	while (1) {
        if (fun1_iter == 0) mutex_lock(mutex);
        
		putchar((int)str);
        
        fun1_iter++;
        
        if (fun1_iter == 5) {
            fun1_iter = 0;
            mutex_unlock(mutex);
        }
        
		if (event_wait(0) < 0) panic("Dev 0 failed");
	}
}

void fun2(void* str) {
	while(1) {
        if (fun2_iter == 0) mutex_lock(mutex);
        
		putchar((int)str);
        
        fun2_iter++;
        
        if (fun2_iter == 3) {
            fun2_iter = 0;
            mutex_unlock(mutex);
        }
        
		if (event_wait(1) < 0) panic("Dev 1 failed");
	}
}

int main(int argc __attribute((unused)),
         char** argv __attribute((unused)))
{
	task_t tasks[2];
	tasks[0].lambda = fun1;
	tasks[0].data = (void*)'@';
	tasks[0].stack_pos = (void*)0x12000000;
	tasks[0].C = 1;
	tasks[0].T = PERIOD_DEV0;
	tasks[1].lambda = fun2;
	tasks[1].data = (void*)'<';
	tasks[1].stack_pos = (void*)0x11000000;
	tasks[1].C = 1;
	tasks[1].T = PERIOD_DEV1;
    
    mutex = mutex_create();
    
	task_create(tasks, 2);

	puts("Why did your code get here!\n");
	return 0;
}
