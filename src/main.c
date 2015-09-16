#include <stdio.h>
#include <stdlib.h>

#include "pidata.h"
#include "pithread.h"

#include "scheduler.h"

#define MAX 100

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


int main(int argc, char* argv[])
{
	//printf("\n Criando thread... \n");
	//printf("eitanui %d", *a);

	int* a = (int*) malloc(sizeof(int));
	int value = 10;
	a = &value;
	
	picreate(91, &inc, a);
	picreate(91, &inc, a);
	picreate(91, &inc, a);
	picreate(91, &inc, a);
	picreate(91, &inc, a);
	picreate(91, &inc, a);
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



	piyield();
	return 0;
}