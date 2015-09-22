#include <stdio.h>
#include <stdlib.h>

#include "pidata.h"
#include "pithread.h"

#include "scheduler.h"

#define MAX 100

bool debug = true;

pimutex_t ahoy_mutex;
pimutex_t another_mutex;

void* inc(void* param)
{
	printf("mutex_test chamado!\n");

	int* i = param;
	
	printf("parametro passado: %i\n", *i);
	
	*i = *i + 1;

	piyield();
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
	// debug = true;


	int* a = (int*) malloc(sizeof(int));
	*a = 10;
	
	// pimutex_init(&ahoy_mutex);
	// pimutex_init(&another_mutex);

	// int tid = picreate(25, &mutex_test2, a);
	// printf("chamando piwait\n");
	// int result = piwait(tid);
	// printf("main: piwait chamado! \t resultado: %i\n", result);

	picreate(100, &inc, a);
	picreate(95, &inc, a);
	picreate(91, &inc, a);
	
	picreate(92, &inc, a);
	picreate(93, &inc, a);
	picreate(94, &inc, a);

	picreate(95, &inc, a);
	
	// int tid = picreate(20, &inc, a);
	// picreate(97, &inc, a);
	// picreate(98, &inc, a);
	// picreate(98, &inc, a);
	// picreate(99, &inc, a);

	piyield();

	piwait(tid);
	free(a);
	
	return 0;
}
