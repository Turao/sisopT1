#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "pidata.h"
#include "pithread.h"

#include "list.h"

#define PRIORITY_DECREMENT 10
#define UNBLOCK_INCREMENT 20
#define CREDITS_MAX 100
#define CREDITS_MIN 0

#define NEW_APT_LIST {NEW_LIST, NEW_LIST, NEW_LIST}

enum {
	CRIACAO = 0,
	APTO = 1,
	EXECUCAO = 2,
	BLOQUEADO = 3,
	TERMINO = 4
};

typedef struct Apts {
	List highPriorityQueue;
	List mediumPriorityQueue;
	List lowPriorityQueue;
} AptList;

/* Scheduler main functions */
void runThread(TCB_t* oldRunning, TCB_t* threadToRun);
void setRunningThread(TCB_t* thread);
// void scheduler();
TCB_t* getNextThread();
void* terminateThread();

void enqueueActive(TCB_t* thread);
void enqueueExpired(TCB_t* thread);
void enqueue(AptList* aptList, TCB_t* thread);

/* AptList related */
TCB_t* apt_takeByTID(AptList* aptList, int tid);

/* 'Attr' getters */
int getNewTID();
TCB_t* getRunningThread();

/* Print functions */
void printThread(TCB_t* thread);
void printAptos();

//Funções que removem a thread atual do "estado" de running
int deactivateRunningThread();
int expireRunningThread();
int blockRunningThread();
int unblockThread(int tid);

#endif // __SCHEDULER__