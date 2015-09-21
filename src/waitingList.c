#include <stdio.h>
#include <stdlib.h>
#include "pidata.h"

#include "waitingList.h"

/* Inicia os atributos de uma lista de espera
*  com ponteiros NULL e tamanho 0
*/
void waitingList_init(WaitingList* waitingList)
{
	waitingList->first = NULL;
	waitingList->last = NULL;
	waitingList->size = 0;
}


/* Retorna True caso seja uma lista de espera vazia,
*  False caso contrario
*/
bool waitingList_isEmpty(WaitingList* waitingList)
{
	if(waitingList == NULL) return false;
	if(waitingList->size == 0) return true;
	else return false;
}


/* Adiciona uma 'informacao de espera' ao final da lista de espera */
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


/* Ve se uma tcb esta sendo esperada por outra
*  retorna a 'informacao de espera' caso seja,
*  retorna NULL caso nao exista a tcb na lista de espera
*/
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


/* Ve se uma tcb esta esperando outra
*  retorna a 'informacao de espera' caso seja,
*  retorna NULL caso nao exista a tcb na lista de espera
*/
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