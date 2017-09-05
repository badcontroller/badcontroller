#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

sem_t sem;
int accesses[5];


static void *inc(void *var)
{
	int i;
	for (i = 0; i < 100; i++) {
		sem_wait(&sem);
		accesses[(int)var]++;
		printf("thread %d using resouce.\n", (int)var);
		usleep(1000000);
		
		sem_post(&sem);
		// printf("thread %d released resouce.\n", (int)var);
		printf ("%d:%d:%d:%d:%d\n", accesses[0],
			accesses[1], accesses[2], accesses[3], accesses[4]);
	}
	
	return NULL;
}

int main()
{
	// 0: shared between threads in this process
	// 3: initial value for semaphore
	sem_init(&sem, 0, 3);
	int var = 0;

	pthread_t t1, t2, t3, t4, t5;
	pthread_create(&t1, NULL, inc, 0);
	pthread_create(&t2, NULL, inc, 1);
	pthread_create(&t3, NULL, inc, 2);
	pthread_create(&t4, NULL, inc, 3);
	pthread_create(&t5, NULL, inc, 4);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	pthread_join(t5, NULL);

	return 0;
}


/*
waits

yes
*/