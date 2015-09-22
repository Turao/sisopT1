#include <stdio.h>
#include <stdlib.h>

#include "pidata.h"
#include "pithread.h"

#include "scheduler.h"

#define MAX 100

pimutex_t ahoy_mutex;
pimutex_t another_mutex;

void* inc(void* param)
{
	printf("mutex_test chamado!\n");

	int* i = param;
	
	printf("parametro passado: %i\n", *i);
	
	*i = *i + 1;

	// piyield();
	printf("inc pos yield \n");
	return NULL;
}

void* mutex_test2(void* param)
{
	// int* a = (int*) malloc(sizeof(int));
	// *a = 10;
	// int tid = picreate(10, &mutex_test, a);
	// int result = piwait(tid);
	// printf("mutex_test2: piwait chamado! \t resultado: %i\n", result);
	
	// int* i = param;
	// pilock(&ahoy_mutex);
	// printf("parametro passado: %i\n", *i);
	// pilock(&another_mutex);
	// printf("lock 2\n");
	// piunlock(&another_mutex);
	// piunlock(&ahoy_mutex);

	return NULL;
}


int main(int argc, char* argv[])
{
	int* a = (int*) malloc(sizeof(int));
	*a = 10;
	
	// pimutex_init(&ahoy_mutex);
	// pimutex_init(&another_mutex);

	// int tid = picreate(25, &mutex_test2, a);
	// printf("chamando piwait\n");
	// int result = piwait(tid);
	// printf("main: piwait chamado! \t resultado: %i\n", result);

	int tid0 = picreate(100, &inc, a);
	int tid1 = picreate(95, &inc, a);
	int tid2 = picreate(91, &inc, a);
	
	int tid3 = picreate(92, &inc, a);
	int tid4 = picreate(93, &inc, a);
	int tid5 = picreate(94, &inc, a);

	// int tid6 = picreate(95, &inc, a);
	
	piwait(tid0);

	free(a);
	
	return 0;
}
