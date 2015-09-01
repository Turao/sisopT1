#include <stdio.h>
#include <stdlib.h>

#include "pidata.h"
#include "pithread.h"

#define MAX 100

void* inc(void* param)
{
	int* actualParam = param;
	*actualParam += 1;
	printf("\nparam: %i", *actualParam);
}


int main(int argc, char* argv[])
{
	//printf("\n Criando thread... \n");

	int *a = (int*) malloc(sizeof(int));
	*a = 1;
	printf("eitanui %d", *a);

	
	picreate(66, &inc, a);
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