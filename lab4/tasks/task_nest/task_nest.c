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

void fun1(void* str);
void fun2(void* str);
void fun3(void* str);
void fun4(void* str);

void panic(const char* str)
{
	puts(str);
	while(1);
}

void fun1(void* str)
{
    putchar((int)str);
    
	task_t tasks[2];
	tasks[0].lambda = fun3;
	tasks[0].data = (void*)'!';
	tasks[0].stack_pos = (void*)0xa2000000;
	tasks[0].C = 1;
    tasks[0].B = 0;
	tasks[0].T = PERIOD_DEV1;
	tasks[1].lambda = fun4;
	tasks[1].data = (void*)'#';
	tasks[1].stack_pos = (void*)0xa1000000;
	tasks[1].C = 1;
    tasks[1].B = 0;
	tasks[1].T = PERIOD_DEV2;

	task_create(tasks, 2);

	puts("Why did your code get here!\n");
}

void fun2(void* str)
{
	while(1)
	{
		putchar((int)str);
		if (event_wait(1) < 0)
			panic("Dev 1 failed");
	}
}

void fun3(void* str)
{
	while(1)
	{
		putchar((int)str);
		if (event_wait(1) < 0)
			panic("Dev 1 failed");
	}
}

void fun4(void* str)
{
	while(1)
	{
		putchar((int)str);
		if (event_wait(2) < 0)
			panic("Dev 2 failed");
	}
}

int main(int argc __attribute((unused)),
         char** argv __attribute((unused)))
{
	task_t tasks[2];
	tasks[0].lambda = fun1;
	tasks[0].data = (void*)'@';
	tasks[0].stack_pos = (void*)0xa2000000;
	tasks[0].C = 1;
    tasks[0].B = 0;
	tasks[0].T = PERIOD_DEV0;
	tasks[1].lambda = fun2;
	tasks[1].data = (void*)'<';
	tasks[1].stack_pos = (void*)0xa1000000;
	tasks[1].C = 1;
    tasks[1].B = 0;
	tasks[1].T = PERIOD_DEV1;

	task_create(tasks, 2);

	puts("Why did your code get here!\n");
	return 0;
}
