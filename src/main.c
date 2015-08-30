#include <stdio.h>
#include <stdlib.h>

#include "pidata.h"
#include "pithread.h"

#define MAX 100


int main(int argc, char* argv[])
{
	//printf("\n Criando thread... \n");

	picreate(66, NULL, NULL);
	picreate(68, NULL, NULL);
	picreate(25, NULL, NULL);
	picreate(42, NULL, NULL);
	picreate(0, NULL, NULL);
	picreate(11, NULL, NULL);
	picreate(100, NULL, NULL);
	picreate(98, NULL, NULL);
	picreate(99, NULL, NULL);

	return 0;
}