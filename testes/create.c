#include <stdlib.h>
#include <stdio.h>

#include "../include/pithread.h"

void* myFunc()
{
	printf("myFunc created!\n");
	return NULL;
}

int main()
{
	int tid = picreate(20, &myFunc, NULL);
	if(tid > 0)	printf("Thread criada com TID: %i\n", tid);

	int tid1 = picreate(-1, &myFunc, NULL);
	if(tid1 > 0)	printf("Thread criada com TID: %i\n", tid1);

	int tid2 = picreate(0, &myFunc, NULL);
	if(tid2 > 0)	printf("Thread criada com TID: %i\n", tid2);

	int tid3 = picreate(100, &myFunc, NULL);
	if(tid3 > 0)	printf("Thread criada com TID: %i\n", tid3);

	int tid4 = picreate(101, &myFunc, NULL);
	if(tid4 > 0)	printf("Thread criada com TID: %i\n", tid4);

	return 0;
}