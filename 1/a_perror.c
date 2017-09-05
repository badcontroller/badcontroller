#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void allocate(int value) {
	int *ptr = NULL;
	ptr = malloc(1024*1024*sizeof(char));
	if (ptr == NULL) {
		perror("allocate failed with following error");
		return;
	}
	*ptr = value;
	printf("test of allocated memory: %i\n", *ptr);
}


int main()
{
	while(1) {
		allocate(10);
	}
	return 0;
}

