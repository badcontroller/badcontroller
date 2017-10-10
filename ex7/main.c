#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <native/timer.h>
#include <sys/mman.h>
#include <native/intr.h>
#include <native/task.h>
#include <native/sem.h>
#include <native/mutex.h>

#define US 1000
#define MS 1000000
#define SEC 1000000000

// time unit = 0.5s
#define TIME_UNIT 100

RT_SEM sem_barrier, sem_resource;
RT_MUTEX mut_resource;


void busy_wait_ms(unsigned long delay) {
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*100);
		count++;
	}
}

int rt_task_sleep_ms(unsigned long delay){
	return rt_task_sleep(1000*1000*delay);
}

static void *low(void *var)
{
	rt_sem_p(&sem_barrier, TM_INFINITE);

	rt_printf("low started\n");
	// rt_sem_p(&sem_resource, TM_INFINITE);
	rt_mutex_acquire(&mut_resource, TM_INFINITE);
	rt_printf("low acquired resource\n");
	busy_wait_ms(1*TIME_UNIT);
	rt_printf("low releasing resource\n");
	// rt_sem_v(&sem_resource);
	rt_mutex_release(&mut_resource);
}

static void *mid(void *var)
{
	rt_sem_p(&sem_barrier, TM_INFINITE);

	rt_task_sleep_ms(1*TIME_UNIT);
	rt_printf("mid starting\n");
	busy_wait_ms(5*TIME_UNIT);
	rt_printf("mid finished\n");
}

static void *high(void *var)
{
	rt_sem_p(&sem_barrier, TM_INFINITE);
	rt_task_sleep_ms(2*TIME_UNIT);
	rt_printf("high starting, waiting\n");
	// rt_sem_p(&sem_resource, TM_INFINITE);
	rt_mutex_acquire(&mut_resource, TM_INFINITE);
	rt_printf("high acquired resource\n");
	busy_wait_ms(2*TIME_UNIT);
	// rt_sem_v(&sem_resource);
	rt_mutex_release(&mut_resource);
	rt_printf("high released resource\n");
}

int main()
{
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	rt_sem_create(&sem_barrier, "sem_barrier", 0, S_PRIO);
	// rt_sem_create(&sem_resource, "sem_resource", 1, S_FIFO);
	rt_mutex_create(&mut_resource, "mut_resouce");

	RT_TASK t_low, t_mid, t_high;
	rt_task_create(&t_low, "task_low", 0, 1, T_CPU(0) | T_JOINABLE);
	rt_task_create(&t_mid, "task_mid", 0, 2, T_CPU(0) | T_JOINABLE);
	rt_task_create(&t_high, "task_high", 0, 3, T_CPU(0) | T_JOINABLE);
	rt_task_start(&t_low, low, 1);
	rt_task_start(&t_mid, mid, 2);
	rt_task_start(&t_high, high, 3);

	rt_task_shadow(NULL, "task_main", 5, T_CPU(0) | T_JOINABLE);

	rt_task_sleep(100*MS);
	rt_printf("main broadcast\n------------------\n\n");
	rt_sem_broadcast(&sem_barrier);
	rt_task_sleep(100*MS);

	// rt_printf("waiting for join\n");
	rt_task_join(&t_low);
	rt_task_join(&t_mid);
	rt_task_join(&t_high);

	rt_printf("deleting semaphores\n");
	rt_sem_delete(&sem_barrier);
	rt_sem_delete(&sem_resource);
	rt_sem_delete(&mut_resource);
	return 0;
}
