#ifndef __WAITINGLIST__
#define __WAITINGLIST__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pidata.h"

#define NEW_WAITING_INFO {NULL, NULL, NULL, NULL}
#define NEW_WAITING_LIST {NULL, NULL, 0}


/* Informacao de espera:
*  contem a informacao das threads em espera
*  possuindo a thread que esta esperando (waiting)
*  e a thread que esta sendo esperada (beingWaited)
*/
typedef struct WaitingInfo
{
	TCB_t* waiting;
	TCB_t* beingWaited;
	struct WaitingInfo* next;
	struct WaitingInfo* prev;
} WaitingInfo;


/* Lista de espera:
*  Contendo as 'informacoes de espera'
*  das threads em espera/sendo esperadas
*/
typedef struct WaitingList
{
	WaitingInfo *first, *last;
	int size;
} WaitingList;


/* Inicia os atributos de uma lista de espera
*  com ponteiros NULL e tamanho 0
*/
void waitingList_init(WaitingList* waitingList);

/* Retorna True caso seja uma lista de espera vazia,
*  False caso contrario
*/
bool waitingList_isEmpty(WaitingList* waitingList);


/* Adiciona uma 'informacao de espera' ao final da lista de espera */
void waitingList_append(WaitingList* waitingList, WaitingInfo* waitingInfo);


/* Ve se uma tcb esta sendo esperada por outra
*  retorna a 'informacao de espera' caso seja,
*  retorna NULL caso nao exista a tcb na lista de espera
*/
WaitingInfo* waitingList_isBeingWaited(WaitingList* waitingList, TCB_t* tcb);


/* Ve se uma tcb esta esperando outra
*  retorna a 'informacao de espera' caso seja,
*  retorna NULL caso nao exista a tcb na lista de espera
*/
WaitingInfo* waitingList_isWaiting(WaitingList* waitingList, TCB_t* tcb);

#endif // __WAITINGLIST__