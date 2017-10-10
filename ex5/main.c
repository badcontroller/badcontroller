#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include "io.h"

int var1 = 0;
int var2 = 0;
int running = 1;

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
	set_cpu(0);
	int channel = (int)var;
	struct timespec t;
	
	while (1) {
		if (io_read(channel) == 0) {
			io_write(channel, 0);
			usleep(5);
			io_write(channel, 1);
		}
		clock_gettime(CLOCK_REALTIME, &t);
		timespec_add_us(&t, 10);
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t, NULL);
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
	pthread_t t1, t2, t3;
	pthread_create(&t1, NULL, thread, 1);
	pthread_create(&t2, NULL, thread, 2);
	pthread_create(&t3, NULL, thread, 3);

	pthread_t t;
	int i;
	for (i = 0; i < 10; i++) {
		pthread_create(t, NULL, disturbance, NULL);
	}
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	
	return 0;
}