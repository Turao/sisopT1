#include <stdio.h>
#include <stdlib.h>

#include "pidata.h"
#include "pithread.h"

#include "scheduler.h"

#define MAX 100

bool debug = true;

void* inc(void* param)
{
	printf("INC chamado!\t");
	int* actualParam = param;
	*actualParam += 1;
	printf("\t parametro passado: %i\n", *actualParam);

	//piyield();
	printThread(getRunningThread());
	printf("running pointer %p\n",getRunningThread());
	// printf("\ninc pos-piyield!\n");

	//piyield();

	return NULL;
}

void* ahoy(void* param)
{
	printf("ahoy chamado!\t");
	//int* actualParam = param;
	// printf("\t parametro passado: %i\n", *actualParam);

	//piyield();
	printThread(getRunningThread());
	printf("running pointer %p\n",getRunningThread());
	// printf("\nahoy pos-piyield!\n");

	//piyield();

	return NULL;
}
	pimutex_t ahoy_mutex;
	pimutex_t another_mutex;
void* mutex_test(void* param)
{
	printf("mutex_test chamado!\n");

	int* i = param;
	pilock(&ahoy_mutex);
	printf("parametro passado: %i\n", *i);
	pilock(&another_mutex);
	printf("lock 2\n");
	piunlock(&another_mutex);
	piunlock(&ahoy_mutex);

	return NULL;
}

void* mutex_test2(void* param)
{
	int* a = (int*) malloc(sizeof(int));
	*a = 10;
	int tid = picreate(10, &mutex_test, a);
	int result = piwait(tid);
	printf("mutex_test2: piwait chamado! \t resultado: %i\n", result);
	
	int* i = param;
	pilock(&ahoy_mutex);
	printf("parametro passado: %i\n", *i);
	pilock(&another_mutex);
	printf("lock 2\n");
	piunlock(&another_mutex);
	piunlock(&ahoy_mutex);

	return NULL;
}


int main(int argc, char* argv[])
{
	//printf("\n Criando thread... \n");
	//printf("eitanui %d", *a);
	debug = true;


	int* a = (int*) malloc(sizeof(int));
	*a = 10;
	
	pimutex_init(&ahoy_mutex);
	pimutex_init(&another_mutex);

	// picreate(100, &mutex_test, a);
	// picreate(90, &mutex_test2, a);
	// picreate(80, &mutex_test2, a);
	// picreate(70, &mutex_test, a);
	// picreate(60, &mutex_test2, a);
	// picreate(50, &mutex_test, a);
	// picreate(40, &mutex_test2, a);
	// picreate(30, &mutex_test, a);
	// picreate(20, &mutex_test2, a);
	// picreate(10, &mutex_test, a);


	int tid = picreate(25, &mutex_test2, a);
	printf("chamando piwait\n");
	int result = piwait(tid);
	printf("main: piwait chamado! \t resultado: %i\n", result);


	free(a);
	//piyield();
	return 0;

	//picreate(91, &inc, a);
	//picreate(91, &inc, a);
	//picreate(91, &inc, a);
	//picreate(91, &inc, a);
	//picreate(91, &inc, a);
	//picreate(91, &inc, a);
	// picreate(91, &inc, a);
	// picreate(91, &inc, a);
	// picreate(91, &inc, a);
	// picreate(91, &inc, a);
	// picreate(91, &inc, a);
	// picreate(81, &inc, a);
	// picreate(81, &inc, a);
	// picreate(81, &inc, a);
	// picreate(81, &inc, a);
	// picreate(81, &inc, a);
	// picreate(81, &inc, a);
	// picreate(81, &inc, a);
	// picreate(81, &inc, a);
	// picreate(81, &inc, a);
	
	// picreate(91, &inc, a);
	// picreate(95, &ahoy, a);
	// picreate(91, &inc, a);
	// picreate(93, &inc, a);
	// picreate(97, &inc, a);
	// picreate(90, &inc, a);
	// picreate(97, &inc, a);

	// picreate(100, &inc, a);
	// picreate(98, &inc, a);
	// picreate(100, &inc, a);
	// picreate(54, &inc, a);
	// picreate(42, &inc, a);
	// picreate(15, &inc, a);
	
	// picreate(90, &ahoy, NULL);
	// picreate(99, &inc, NULL);
	// picreate(96, &inc, NULL);
	// picreate(99, &inc, NULL);
	// picreate(86, &ahoy, NULL);
	// picreate(97, &ahoy, NULL);
	// picreate(86, &ahoy, NULL);
	// picreate(99, &ahoy, NULL);

	// picreate(100, &ahoy, NULL);
	// picreate(98, &ahoy, NULL);
	// picreate(99, &inc, NULL);
}
