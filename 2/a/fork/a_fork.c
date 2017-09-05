#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int g_var = 0;

int main()
{
	int var = 0;
	
	fork();
	
	int i;
	for (i = 0; i < 10; i++) {
		var++;
		g_var++;
	}
	
	printf("var: %d\n", var);
	printf("g_v: %d\n", g_var);
	
	return 0;
}

/*
fork() lager ny child som er kopi av original
vfork() lager ny child, men som deler minne,
altså viktig å ikke endre vars eller returnere fra funksjon
*/