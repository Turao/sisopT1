#include <stdlib.h>
#include <stdio.h>

void* myFunc()
{
	printf("myFunc created!\n");
	return NULL;
}

int main()
{
	int tid = picreate(20, &myFunc, NULL);
	printf("Thread criada com TID: %i\n", tid);

	return 0;
}