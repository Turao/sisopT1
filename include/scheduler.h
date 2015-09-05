#include "pidata.h"
#include "pithread.h"

enum {CRIACAO = 0, APTO, EXECUCAO, BLOQUEADO, TERMINO};


/* Scheduler main functions */
// void scheduler();
void* terminateThread();
void enqueue(TCB_t* thread);

/* 'Attr' getters */
int getNewTID();
TCB_t* getRunningThread();

/* Print functions */
void printThread(TCB_t* thread);
void printAptos();