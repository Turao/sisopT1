#include "pidata.h"
#include "pithread.h"

#include "list.h"

#define PRIORITY_DECREMENT 10

#define NEW_APT_LIST {NEW_LIST, NEW_LIST, NEW_LIST}

enum {CRIACAO = 0, APTO, EXECUCAO, BLOQUEADO, TERMINO};

typedef struct Apts {
	List highPriorityQueue;
	List mediumPriorityQueue;
	List lowPriorityQueue;
} AptList;

/* Scheduler main functions */
// void scheduler();
TCB_t* getNextThread();
void* terminateThread();

void enqueueActive(TCB_t* thread);
void enqueueExpired(TCB_t* thread);
void enqueue(AptList* aptList, TCB_t* thread);

/* 'Attr' getters */
int getNewTID();
TCB_t* getRunningThread();

/* Print functions */
void printThread(TCB_t* thread);
void printAptos();

//Funções que removem a thread atual do "estado" de running
int deactivateRunningThread();
int expireRunningThread();