#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int var1 = 0;
int var2 = 0;
int running = 1;

static void *thread_1(void *var)
{
	while (running) {
		pthread_mutex_lock(&mutex);
		var1++;
		var2 = var1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

static void *thread_2(void *var)
{
	int i;
	for (i = 0; i < 20; i++) {
		//pthread_mutex_lock(&mutex);
		printf("Number 1 is %d, number 2 is %d\n", var1, var2);
		(var1 == var2) ? printf("OK\n") : printf("MISS\n");
		//pthread_mutex_unlock(&mutex);
		usleep(100000);
	}
	running = 0;
	return NULL;
}

int main()
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, thread_1, 1);
	pthread_create(&t2, NULL, thread_2, 2);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	return 0;
}

/*
preempted underveis

tillegg: måtte legge til mutex på read også
*/