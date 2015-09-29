#include <stdlib.h>
#include <stdio.h>

#include "../include/pithread.h"

void* calcFunc();
void* mutexFunc1();
void* mutexFunc2();

int *i;
pimutex_t mutex;

void* calcFunc()
{
	printf("\t\ti in calcFunc = %d\n", ++(*i));
	return NULL;
}

void* mutexFunc1()
{
	int tid = picreate(30, &calcFunc, NULL);
	printf("\t\tentrando em mutexFunc1!\n");
	pilock(&mutex);

	printf("\t\tchamando piwait em mutexFunc1!\n");
	piwait(tid);
	printf("\t\ti = %d\n", *i);

	piunlock(&mutex);
	printf("\t\t * terminando mutexFunc1!\n");

	return NULL;
}
void* mutexFunc2()
{
	int tid = picreate(30, &calcFunc, NULL);
	printf("\t\tentrando em mutexFunc2!\n");
	pilock(&mutex);

	printf("\t\tchamando piwait em mutexFunc2!\n");
	piwait(tid);
	printf("\t\ti = %d\n", *i);

	piunlock(&mutex);
	printf("\t\t * terminando mutexFunc2!\n");

	return NULL;
}

int main()
{
	int tid1 = picreate(30, &mutexFunc1, NULL);
	int tid2 = picreate(30, &mutexFunc2, NULL);
	i = (int*) malloc(sizeof(int));
	*i = 0;

	if(pimutex_init(&mutex) == 0) printf("\n\t\t* mutex init OK! *\t\t\n");

	piwait(tid1);
	piwait(tid2);
	printf("\t\tterminating main\n");

	return 0;
}
