#include <pthread.h>
#include <stdio.h>

int g_var = 0;

static void *inc(void *var)
{
	g_var++;
	(*(int*)var)++;

	int i;
	for (i = 0; i < 10000; i++)
		(*(int*)var)++;
	return NULL;
}

int main()
{
	int var = 0;

	pthread_t t1, t2;

	pthread_create(&t1, NULL, inc, &var);
	pthread_create(&t2, NULL, inc, &var);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("local: %d\n", var);
	printf("globl: %d\n", g_var);

	return 0;
}

/*
globale er delt
lokale er på stack, og da ikke delt
i tilfellet vårt passerer vi en peker, derfor
får den endret
*/