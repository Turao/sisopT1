#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "pithread.h"

#include "pidata.h"

#include "aptList.h"

#define SUCCESS 0
#define ERROR -1

AptList aptos_ativos = NEW_APT_LIST;
AptList aptos_expirados = NEW_APT_LIST;


int picreate (int credCreate, void* (*start)(void*), void *arg)
{
	/**************************************************************/
	printf(" **************************************************** \n");
	printf(" * Aptos Ativos: %i \t\t", aptos_ativos.size);
	printf(" * Aptos Expirados: %i \n", aptos_expirados.size);
	printf(" **************************************************** \n");
	printf(" \t\tCriando nova thread...   \n");
	//printf(" **************************************************** \n");
	/**************************************************************/


	TCB_t* newThread = (TCB_t*) malloc(sizeof(TCB_t));

	newThread->tid = aptos_ativos.size + aptos_expirados.size;
	newThread->state = 1;
	newThread->credCreate = credCreate;

	newThread->credReal = credCreate;
	getcontext(&newThread->context);
	//makecontext(&newThread->context, (void (*)(void)) start, (int) arg);

	//append(&aptos_ativos, newThread);
	apt_add(&aptos_ativos, newThread);

	/**************************************************************/
	printf(" **************************************************** \n");
	printf(" * Aptos Ativos: %i \t\t", aptos_ativos.size);
	printf(" * Aptos Expirados: %i \n", aptos_expirados.size);
	printf(" **************************************************** \n");
	/**************************************************************/

	/**************************************************************/
	printf("\n");
	printf(" * Thread TID: %i \n", newThread->tid);
	printf(" * Thread state: %i \n", newThread->state);
	printf(" * Thread credits: %i \n", newThread->credCreate);
	printf(" * Thread dynamic credits: %i \n", newThread->credReal);

	if(newThread->prev != NULL) printf(" * Prev Thread TID: %i \n", newThread->prev->tid);
	else printf(" * Prev Thread TID: NULL \n"); 

	if(newThread->next != NULL) printf(" * Next Thread TID: %i \n", newThread->next->tid);
	else printf(" * Next Thread TID: NULL \n");	

	printf("\n");
	/**************************************************************/	

	//free(newThread);

	printf("\n");
	apt_print(aptos_ativos);
	printf("\n");

	return SUCCESS;
}

int piyield(void)
{
	return SUCCESS;
}

int piwait(int tid)
{
	return SUCCESS;
}

int pimutex_init(pimutex_t *mtx)
{
	return SUCCESS;
}

int pilock (pimutex_t *mtx)
{
	return SUCCESS;
}

int piunlock (pimutex_t *mtx)
{
	return SUCCESS;
}