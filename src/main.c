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

	piyield();

	printf("\ninc pos-piyield!\n");

	piyield();

	return NULL;
}

void* ahoy(void* param)
{
	printf("ahoy chamado!\t");
	int* actualParam = param;
	printf("\t parametro passado: %i\n", *actualParam);

	piyield();

	printf("\nahoy pos-piyield!\n");

	piyield();

	return NULL;
}


int main(int argc, char* argv[])
{
	//printf("\n Criando thread... \n");

	int *a = (int*) malloc(sizeof(int));
	*a = 1;
	//printf("eitanui %d", *a);

	
	picreate(91, &inc, a);
	picreate(91, &inc, a);
	picreate(95, &ahoy, a);
	picreate(91, &inc, a);
	picreate(93, &inc, a);
	picreate(97, &inc, a);
	picreate(90, &inc, a);
	// picreate(97, &inc, a);
	piyield();

	// picreate(100, &inc, a);
	// picreate(98, &inc, a);
	// picreate(100, &inc, a);
	// picreate(54, &inc, a);
	// picreate(42, &inc, a);
	// picreate(15, &inc, a);
	// picreate(68, NULL, NULL);
	// picreate(25, NULL, NULL);
	// picreate(25, NULL, NULL);
	// picreate(42, NULL, NULL);
	// picreate(0, NULL, NULL);
	// picreate(11, NULL, NULL);
	// picreate(100, NULL, NULL);
	// picreate(98, NULL, NULL);
	// picreate(99, NULL, NULL);

	return 0;
}