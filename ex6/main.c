#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "io.h"
#include <native/timer.h>
#include <sys/mman.h>
#include <native/intr.h>
#include <native/task.h>

#define US 1000
#define MS 1000000
#define SEC 1000000000

void fn(void* cookie) {
	while(1) {
		printf("Hello!\n");
		rt_task_wait_period(NULL);
	}
}

int set_cpu(int cpu_number)
{
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void timespec_add_us(struct timespec *t, long us)
{
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;
	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000)
	{
		t->tv_nsec = t->tv_nsec - 1000000000;
		t->tv_sec += 1;
	}
}

static void *thread(void *var)
{
	int channel = (int)var;
	
	while (1) {
		if (io_read(channel) == 0) {
			io_write(channel, 0);
			rt_task_sleep(5);
			io_write(channel, 1);
		}
		rt_task_wait_period(NULL);
	}
	return NULL;
}

static void *disturbance(void *var)
{
	set_cpu(0);
	float x = 0;
	while (1) {
		x += 1;
	}
	return NULL;
}

int main()
{
	io_init();

	pthread_t t;
	int i;
	for (i = 0; i < 10; i++) {
		pthread_create(t, NULL, disturbance, NULL);
	}
	
	mlockall(MCL_CURRENT|MCL_FUTURE);
	RT_TASK t1, t2, t3;
	int period = 10;
	rt_task_create(&t1, "task1", 0, 1, T_CPU(0) | T_JOINABLE);
	rt_task_create(&t2, "task2", 0, 1, T_CPU(0) | T_JOINABLE);
	rt_task_create(&t3, "task3", 0, 1, T_CPU(0) | T_JOINABLE);
	rt_task_set_periodic(&t1, TM_NOW, period * US);
	rt_task_set_periodic(&t2, TM_NOW, period * US);
	rt_task_set_periodic(&t3, TM_NOW, period * US);
	rt_task_start(&t1, thread, 1);
	rt_task_start(&t2, thread, 2);
	rt_task_start(&t3, thread, 3);

	rt_task_join(&t1);
	rt_task_join(&t2);
	rt_task_join(&t3);
	// rt_task_delete(&task);
	return 0;
}
