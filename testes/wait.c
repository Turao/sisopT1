#include <stdlib.h>
#include <stdio.h>

void* myFunc();
void* anotherFunc();

void* myFunc()
{
	printf("myFunc created!\n");

	int anotherTID = picreate(1, &anotherFunc, NULL);

	piwait(anotherTID);
	printf("terminating myFunc\n");
	return NULL;
}

void* anotherFunc()
{
	printf("anotherFunc created!\n");
	printf("terminating anotherFunc\n");
	return NULL;
}

int main()
{
	int tid = picreate(20, &myFunc, NULL);

	piwait(tid);
	printf("terminating main\n");

	return 0;
}