/** @file dagger.c
 *
 * @brief Creates two simple periodic tasks.
 *
 * @note  This is like knife -- but smaller :)
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-30
 */
#include <stdio.h>
#include <task.h>
#include <unistd.h>

int mutex = 0;
int mutex1 = 0;
int mutex2 = 0;
int fun1_iter = 0;
int fun2_iter = 0;
int fun3_iter = 0;

void panic(const char* str)
{
	puts(str);
	while(1);
}

// Expected output with HLP: 333444422 ... (2 must always come after 4)

void fun1(void* str) {
	while (1) {
        if (fun1_iter == 0) mutex_lock(mutex);
        
		putchar((int)str);
        
        fun1_iter++;
        
        if (fun1_iter == 3) {
            fun1_iter = 0;
            mutex_unlock(mutex);
            if (event_wait(2) < 0) panic("Dev 2 failed");
        } else {
            sleep(500);
        }
	}
}

void fun2(void* str) {
	while(1) {
        if (fun2_iter == 0) mutex_lock(mutex2);
        
		putchar((int)str);
        
        fun2_iter++;
        
        if (fun2_iter == 4) {
            fun2_iter = 0;
            mutex_unlock(mutex2);
            if (event_wait(2) < 0) panic("Dev 0 failed");
        } else {
            sleep(80);
        }
	}
}

void fun3(void* str) {
	while(1) {
        if (fun3_iter == 0) mutex_lock(mutex1);
        
		putchar((int)str);
        
        fun3_iter++;
        
        if (fun3_iter == 2) {
            fun3_iter = 0;
            mutex_unlock(mutex1);
            if (event_wait(2) < 0) panic("Dev 3 failed");
        } else {
            sleep(200);
        }
	}
}

int main(int argc __attribute((unused)),
         char** argv __attribute((unused)))
{
	task_t tasks[3];
	tasks[0].lambda = fun1;
	tasks[0].data = (void*)'3';
	tasks[0].stack_pos = (void*)0xa2000000;
	tasks[0].C = 1;
    tasks[0].B = 0;
	tasks[0].T = 50;
	tasks[1].lambda = fun2;
	tasks[1].data = (void*)'4';
	tasks[1].stack_pos = (void*)0xa1000000;
	tasks[1].C = 1;
    tasks[1].B = 0;
	tasks[1].T = 100;
    tasks[2].lambda = fun3;
	tasks[2].data = (void*)'2';
	tasks[2].stack_pos = (void*)0xa1500000;
	tasks[2].C = 1;
    tasks[2].B = 0;
	tasks[2].T = 300;
    
    mutex = mutex_create();
    mutex1 = mutex_create();
    mutex2 = mutex_create();
    
    fun1_iter = 0;
    fun2_iter = 0;
    fun3_iter = 0;
    
	task_create(tasks, 3);

	puts("Why did your code get here!\n");
	return 0;
}
