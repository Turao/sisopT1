#include <stdlib.h>
#include <stdio.h>

void* myFunc()
{

}

int main()
{

	int tid = picreate(20, &myFunc, NULL);


	return 0;
}