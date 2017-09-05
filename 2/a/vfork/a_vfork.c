#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int g_var = 0;

int main()
{
	int var = 0;
	
	vfork();
	
	int i;
	for (i = 0; i < 10; i++) {
		var++;
		g_var++;
	}
	
	printf("var: %d\n", var);
	printf("g_v: %d\n", g_var);
	
	return 0;
}
