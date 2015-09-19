#ifndef __WAITINGLIST__
#define __WAITINGLIST__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pidata.h"

#define NEW_WAITING_INFO {NULL, NULL, NULL, NULL}
#define NEW_WAITING_LIST {NULL, NULL, 0}

typedef struct WaitingInfo
{
	TCB_t* waiting;
	TCB_t* beingWaited;
	struct WaitingInfo* next;
	struct WaitingInfo* prev;
} WaitingInfo;

typedef struct WaitingList
{
	WaitingInfo *first, *last;
	int size;
} WaitingList;

void waitingList_init(WaitingList* waitingList);
bool waitingList_isEmpty(WaitingList* waitingList);
void waitingList_append(WaitingList* waitingList, WaitingInfo* waitingInfo);
WaitingInfo* waitingList_isBeingWaited(WaitingList* waitingList, TCB_t* tcb);
WaitingInfo* waitingList_isWaiting(WaitingList* waitingList, TCB_t* tcb);

#endif // __WAITINGLIST__