#include <stdio.h>
#include <stdlib.h>
#include "pidata.h"

#include "waitingList.h"


/* Inicializa os atributos da lista de threads em espera/esperando */
void waitingList_init(WaitingList* waitingList)
{
	waitingList->first = NULL;
	waitingList->last = NULL;
	waitingList->size = 0;
}

/* Retorna um booleano dizendo se a lista passada é vazia */
bool waitingList_isEmpty(WaitingList* waitingList)
{
	if(waitingList == NULL) return false;
	if(waitingList->size == 0) return true;
	else return false;
}

void waitingList_append(WaitingList* waitingList, WaitingInfo* waitingInfo)
{
	if(waitingList_isEmpty(waitingList))
	{
		waitingList->first = waitingInfo;
		waitingList->last = waitingInfo;
		waitingList->size = 1;

		waitingInfo->prev = NULL;
		waitingInfo->next = NULL;
	}
	else
	{
		WaitingInfo* oldLast = waitingList->last;
		waitingList->last = waitingInfo;
		oldLast->next = waitingInfo;
		waitingList->size += 1;
		waitingInfo->prev = oldLast;
		waitingInfo->next = NULL;
	}
}


// void waitingList_takeByTID(WaitingList* waitingList);

WaitingInfo* waitingList_isBeingWaited(WaitingList* waitingList, TCB_t* tcb)
{
	if(waitingList == NULL) return NULL;
	if(waitingList_isEmpty(waitingList)) return NULL;

	int i = 0;
	WaitingInfo* currentInfo = waitingList->first;
	for(; i < waitingList->size; i++)
	{
		if(currentInfo->beingWaited == tcb) return currentInfo;
		currentInfo = currentInfo->next;
	}
	return NULL;
}

WaitingInfo* waitingList_isWaiting(WaitingList* waitingList, TCB_t* tcb)
{
	if(waitingList == NULL) return NULL;
	if(waitingList_isEmpty(waitingList)) return NULL;

	int i = 0;
	WaitingInfo* currentInfo = waitingList->first;
	for(; i < waitingList->size; i++)
	{
		if(currentInfo->waiting == tcb) return currentInfo;
		currentInfo = currentInfo->next;
	}
	return NULL;
}