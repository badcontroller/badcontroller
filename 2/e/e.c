#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

#define MAX_SEATS 3

pthread_mutex_t forks[MAX_SEATS];
pthread_mutex_t fork1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork3 = PTHREAD_MUTEX_INITIALIZER;

static void *philosopher(void *seat)
{
	printf("seat: %d\n", seat);
	int f_left = seat;
	int f_right = seat+1;
	if (f_right == MAX_SEATS)
		f_right = 0;

	if (seat == 0) {
		pthread_mutex_lock(&forks[f_left]);
		printf("P%d picked up left fork\n", seat);
		pthread_mutex_lock(&forks[f_right]);
		printf("P%d picked up right fork\n", seat);
		printf("P%d eating...\n", seat);
		usleep(10000);
		printf("P%d returning left fork\n", seat);
		pthread_mutex_unlock(&forks[f_left]);
		printf("P%d returning right fork\n", seat);
		pthread_mutex_unlock(&forks[f_right]);
	} else {
		pthread_mutex_lock(&forks[f_right]);
		printf("P%d picked up right fork\n", seat);
		pthread_mutex_lock(&forks[f_left]);
		printf("P%d picked up left fork\n", seat);
		printf("P%d eating...\n", seat);
		usleep(10000);
		printf("P%d returning right fork\n", seat);
		pthread_mutex_unlock(&forks[f_right]);
		printf("P%d returning left fork\n", seat);
		pthread_mutex_unlock(&forks[f_left]);
	}
	
	return NULL;
}

int main()
{
	forks[0] = fork1;
	forks[1] = fork2;
	forks[2] = fork3;
	pthread_t p0, p1, p2;
	while(1) {	
		pthread_create(&p0, NULL, philosopher, 0);
		pthread_create(&p1, NULL, philosopher, 1);
		pthread_create(&p2, NULL, philosopher, 2);
		
		pthread_join(p0, NULL);
		pthread_join(p1, NULL);
		pthread_join(p2, NULL);
	}
	
	return 0;
}