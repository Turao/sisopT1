#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "pidata.h"
#include "pithread.h"

#include "list.h"
#include "waitingList.h"

#define CREDITS_MAX 100
#define CREDITS_MIN 0

#define HIGH_PRIORITY_CREDITS 80
#define MEDIUM_PRIORITY_CREDITS 40
#define LOW_PRIORITY_CREDITS 0

#define PRIORITY_DECREMENT 10
#define UNBLOCK_INCREMENT 20

#define NEW_APT_LIST {NEW_LIST, NEW_LIST, NEW_LIST}

/* Enum de estados possiveis para
*  uma thread
*/
enum {
	CRIACAO = 0,
	APTO = 1,
	EXECUCAO = 2,
	BLOQUEADO = 3,
	TERMINO = 4
};


/* Enum de estados possiveis para
*  um mutex
*/
enum {
	OCUPADO = 0,
	LIVRE = 1
};


/* Estrutura de lista de aptos
*  contendo tres niveis de prioridade
*  PRIORIDADE ALTA: 100~80
*  PRIORIDADE MEDIA: 80~40
*  PRIORIDADE BAIXA: 40~0
*/
typedef struct Apts {
	List highPriorityQueue;
	List mediumPriorityQueue;
	List lowPriorityQueue;
} AptList;




/***********************************************/
/********* SCHEDULER MAIN FUNCTIONS ************/

/* DISPATCHER : runThread 
*  o nome so muda para melhorar a semantica
*  a legibilidade do codigo
*
*  Responsavel pela troca de contexto
*  entre thread em execucao e thread
*  escolhida pelo escalonador
*/
void runThread(TCB_t* oldRunning, TCB_t* threadToRun);

/* SCHEDULER : getNextThread 
*  o nome so muda para melhorar a semantica
*  a legibilidade do codigo
*
*  Responsavel pela escolha da proxima thread
*  atraves de um sistema de filas com multiplas
*  prioridades e politica FILO
*/
TCB_t* getNextThread();

/* Define a thread em execucao e altera 
*  o estado da tcb para EXECUTANDO
*/
void setRunningThread(TCB_t* thread);

/* Responsavel pelo tratamento de uma thread
*  apos o termino de sua execucao
*  libera a memoria e chama o escalonador
*/
void* terminateThread();




/***********************************************/
/********* AptList related functions************/

/* Retorna a lista de aptos ativos */
AptList* getAptosAtivos();

/* Retorna a lista de aptos expirados */
AptList* getAptosExpirados();

/* Insere uma thread na fila de aptos ativos */
void enqueueActive(TCB_t* thread);

/* Insere uma thread na fila de aptos expirados */
void enqueueExpired(TCB_t* thread);

/* Insere uma thread em uma lista de aptos */
void enqueue(AptList* aptList, TCB_t* thread);

/* Encontra a tcb na lista de aptos passada E 
*  remove ela da lista
*  Retorna NULL caso nao encontre
*/
TCB_t* apt_findByTID(AptList* aptList, int tid);

/* Encontra a tcb na lista de aptos passada
*  atraves do seu TID
*  Retorna NULL caso nao encontre
*/
TCB_t* apt_takeByTID(AptList* aptList, int tid);




/***********************************************/
/************* 'Attr' getters ******************/

/* Retorna um TID valido (nao utilizado por outra thread) */
int getNewTID();

/* Retorna a thread em execucao */
TCB_t* getRunningThread();




/***********************************************/
/************* Print functions *****************/

/* Imprime o conteudo de uma thread */
void printThread(TCB_t* thread);

/* Imprime a quantidade de elementos em cada lista de aptos */
void printAptos();

/* Imprime a lista de aptos ativos */
void printAptosLists();




/***********************************************/
/***************** YIELD ***********************/

/* Decrementa o valor especificado em 
*  PRIORITY_DECREMENT da thread em execucao 
*  e a passa para a lista de aptos ativos 
*  para ser escalonada posteriormente
*/
int deactivateRunningThread();

/* Define o valor dos creditos dinamicos da thread 
*  em execucao para o valor dos seus creditos de criacao
*  e a passa para a lista de aptos expirados
*  para ser escalonada posteriormente
*/
int expireRunningThread();




/***********************************************/
/***************** MUTEX ***********************/

int blockThreadForMutex(pimutex_t *mtx, TCB_t* thread);
int unblockMutexThreads(pimutex_t *mtx);




/* WAIT */
/* Bloqueia a thread em execucao, passando-a para a lista de espera */
int blockRunningThread();

/* Bloqueia a thread de TID tid, passando-a para a lista de aptos ativos */
int unblockThread(int tid);

/* Retorna a lista threads em espera */
WaitingList* getWaitingList();

#endif // __SCHEDULER__
