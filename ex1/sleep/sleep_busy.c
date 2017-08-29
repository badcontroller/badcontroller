#include <pthread.h>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>

void busy_wait_delay(int);

void *sleep_print(void *x_void_ptr)
{

	printf("%d: start\n", (int)pthread_self());
	busy_wait_delay(5);
	printf("%d: stopp\n", (int)pthread_self());
}

int main()
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, sleep_print, NULL);
	pthread_create(&t2, NULL, sleep_print, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	return 0;
}

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps))
	{
		for(i=0; i<1000; i++)
		{
			dummy = i;
		}
		times(&exec_time);
	}
}
