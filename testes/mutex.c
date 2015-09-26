#include <stdlib.h>
#include <stdio.h>

#include "../include/pithread.h"

pimutex_t mutex;
int t0;
int t1;

void* sum(void* arg)
{
	printf("entrando em sum!\n");
	pilock(&mutex);
	int *n = (int*) arg;
	int i = 0;
	for(; i < 5; i++)
	{
		*n += 2;
		printf("somando !n: %i\n", *n);
		piyield();
	}
	piunlock(&mutex);
	printf(" * terminando sum!\n");

	return NULL;
}

void* sub(void* arg)
{
	printf("entrando em sub\n");
	pilock(&mutex);
	int *n = (int*) arg;
	int i = 0;
	for(; i < 5; i++)
	{
		*n -= 2;
		printf("subtraindo n: %i\n", *n);
		piyield();
	}
	piunlock(&mutex);
	printf(" * terminando sub!\n");

	return NULL;
}

int main()
{

	int *n = (int*) malloc(sizeof(int));
	*n = 0;

	if(pimutex_init(&mutex) == 0) printf("\n\t\t* mutex init OK! *\t\t\n");

	t0 = picreate(1, &sum, n);
	t1 = picreate(1, &sub, n);
	
	piwait(t0);
	piwait(t1);

	return 0;
}