#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "list.h"

#define SUCCESS 0
#define ERROR -1

int _GLOBAL_TID = 1;

// usar no scheduler, por enquanto esta sendo
// usado so pra testes
AptList* aptos_ativos = NULL;
AptList* aptos_expirados = NULL;

List blocked = NEW_LIST;

TCB_t* runningThread;

WaitingList waitingList = NEW_WAITING_LIST;

int initAptosAtivos()
{
	aptos_ativos = (AptList*) malloc(sizeof(AptList));
	if(aptos_ativos == NULL) 
	{
		// printf("Erro ao alocar lista de aptos ativos!\n");
		return ERROR;
	}
	return SUCCESS;
}

int initAptosExpirados()
{
	aptos_expirados = (AptList*) malloc(sizeof(AptList));
	if(aptos_expirados == NULL) 
	{
		// printf("Erro ao alocar lista de aptos expirados!\n");
		return ERROR;
	}
	return SUCCESS;
}

void runThread(TCB_t* oldRunning, TCB_t* threadToRun)
{
	// printf("Chamando thread de tid: %i\n", threadToRun->tid);
	setRunningThread(threadToRun);
	if(oldRunning != NULL)
	{
		swapcontext(&oldRunning->context, &threadToRun->context);
	}
	else
	{
		setcontext(&threadToRun->context);
	}
}


// pega proxima thread de acordo com 
// a logica imposta pelo trabalho 
//
TCB_t* getNextThread()
{
	TCB_t* nextThread = NULL;

	if(aptos_ativos->highPriorityQueue.size > 0) nextThread = list_popFront(&aptos_ativos->highPriorityQueue);
	else if(aptos_ativos->mediumPriorityQueue.size > 0) nextThread = list_popFront(&aptos_ativos->mediumPriorityQueue);
	else if(aptos_ativos->lowPriorityQueue.size > 0) nextThread = list_popFront(&aptos_ativos->lowPriorityQueue);

	if(nextThread == NULL)
	{
		// nao ha threads na lista de aptos ativos
		// logo, devemos procurar na lista de aptos expirados
		// e caso exista, trocamos os ponteiros.
		// a lista de aptos ativos passa a ser a de aptos expirados
		// e vice versa
		if(aptos_expirados->highPriorityQueue.size > 0) nextThread = list_popFront(&aptos_expirados->highPriorityQueue);
		else if(aptos_expirados->mediumPriorityQueue.size > 0) nextThread = list_popFront(&aptos_expirados->mediumPriorityQueue);
		else if(aptos_expirados->lowPriorityQueue.size > 0) nextThread = list_popFront(&aptos_expirados->lowPriorityQueue);

		if(nextThread != NULL)
		{
			AptList* swap;
			swap = aptos_ativos;
			aptos_ativos = aptos_expirados;
			aptos_expirados = swap;
		}
		// caso nao exista nem na lista de expirados
		// nao existe mais nenhuma thread para ser executada
		// e provavelmente houve algum erro de manipulacao
		// por parte do escalonador
		else
		{
			// printf("Nao existe thread para executar!\n");
			// printf("Erro de manipulacao de threads no escalonador!\n");
			return NULL;
		}
	}

	return nextThread;
}

void setRunningThread(TCB_t* thread)
{
	// e define a thread em questao como executando
	runningThread = thread;
	thread->state = EXECUCAO;
}

// apos o termino da thread, essa
// funcao e chamada,
// nao sei o que deve ser feito por enquanto
// so que devemos chamar o escalonador no final
// pra que ele escolha a proxima thread
void* terminateThread()
{
	TCB_t* runningThread = getRunningThread();
	if(runningThread == NULL) 
	{
		// printf("Nao existe thread em execucao!\n");
		// printf("Erro de manipulacao de threads no escalonador!\n");
		return NULL;
	}
	else
	{

		// printf("\n\t- A thread de tid %3i terminou de executar. -\n", runningThread->tid);

		// checa se a thread estava sendo esperada por outra
		// se estiver, libera a thread na fila de bloqueados
		WaitingInfo* waitingInfo = waitingList_isBeingWaited(&waitingList, runningThread);
		if(waitingInfo != NULL)
		{
			unblockThread(waitingInfo->waiting->tid);
			// printf("\n\t- Desbloqueando thread de TID: %i. -\n", waitingInfo->waiting->tid);
		}

		// libero o espaco de memoria alocado para a
		// thread que terminou e sua stack
		// printf("free\n");
		free(runningThread);

		TCB_t* nextThread = getNextThread();
		if(nextThread != NULL) runThread(NULL, nextThread);

		return NULL;
	}
}

void enqueueActive(TCB_t* thread)
{
	// printf("Thread inserida na fila de aptos ativos.\n");
	if(aptos_ativos == NULL) initAptosAtivos();
	if(aptos_expirados == NULL) initAptosExpirados();

	enqueue(aptos_ativos, thread);
}

void enqueueExpired(TCB_t* thread)
{
	// printf("Thread inserida na fila de aptos expirados.\n");
	if(aptos_ativos == NULL) initAptosAtivos();
	if(aptos_expirados == NULL) initAptosExpirados();

	enqueue(aptos_expirados, thread);
}

// adiciona uma thread a sua fila correspondente
// baseado em seus creditos de criacao
void enqueue(AptList* aptList, TCB_t* thread)
{
	// adiciona a thread a sua respectiva
	// fila, dependendo dos seus creditos
	// de criacao
	// printf("Thread adicionada em: ");
	if(thread->credReal > HIGH_PRIORITY_CREDITS)
	{
		// printf("\t High Priority Queue\n");
		list_add(&aptList->highPriorityQueue, thread);
		// list_print(highPriorityQueue);
	}
	else
	{
		if(thread->credReal > MEDIUM_PRIORITY_CREDITS)
		{
			// printf("\t Medium Priority Queue\n");
			list_add(&aptList->mediumPriorityQueue, thread);
			// list_print(mediumPriorityQueue);
		}
		else
		{
			// printf("\t Low Priority Queue\n");
			list_add(&aptList->lowPriorityQueue, thread);
			// list_print(lowPriorityQueue);
			// printf("\n");
		}
	}
}


// usado para receber um valor valido de TID
int getNewTID()
{
	int tid = _GLOBAL_TID;
	_GLOBAL_TID++;

	return tid;
}

// usado para pegar o ponteiro da thread atual
TCB_t* getRunningThread()
{
	return runningThread;
}

// imprime a quantidade de aptos ativos/expirados
// e seu conteudo
void printAptos()
{
	// int active = aptos_ativos->highPriorityQueue.size +
	// 	aptos_ativos->mediumPriorityQueue.size +
	// 	aptos_ativos->lowPriorityQueue.size;

	// int expired = aptos_expirados->highPriorityQueue.size +
	// 	aptos_expirados->mediumPriorityQueue.size +
	// 	aptos_expirados->lowPriorityQueue.size;

	// printf(" **************************************************** \n");
	// printf(" * Aptos Ativos: %i \t\t", active);
	// printf(" * Aptos Expirados: %i \n", expired);
	// printf(" **************************************************** \n");
}

// imprime o conteudo de uma thread
void printThread(TCB_t* thread)
{
	// if(thread == NULL) printf("\n NULL THREAD \n");
	// printf("\n");
	// printf(" * Thread TID: %i \n", thread->tid);
	// printf(" * Thread state: %i \n", thread->state);
	// printf(" * Thread credits: %i \n", thread->credCreate);
	// printf(" * Thread dynamic credits: %i \n", thread->credReal);

	// if(thread->prev != NULL) printf(" * Prev Thread TID: %i \n", thread->prev->tid);
	// else printf(" * Prev Thread TID: NULL \n"); 

	// if(thread->next != NULL) printf(" * Next Thread TID: %i \n", thread->next->tid);
	// else printf(" * Next Thread TID: NULL \n");	

	// printf("\n");

}

// decrementa os creditos da thread em posicao
// de execucao e coloca ela na fila de aptos ATIVOS
// seta running thread como NULL por seguranca
int deactivateRunningThread()
{
	TCB_t* runningThread = getRunningThread();
	if(runningThread == NULL) return ERROR;

	runningThread->credReal -= PRIORITY_DECREMENT;

	enqueueActive(runningThread);

	runningThread = NULL;

	return SUCCESS;
}

// decrementa os creditos da thread em posicao
// de execucao e coloca ela na fila de aptos EXPIRADOS
// seta running thread como NULL por seguranca
int expireRunningThread()
{
	TCB_t* runningThread = getRunningThread();
	if(runningThread == NULL) return ERROR;

	runningThread->credReal = runningThread->credCreate;
	
	enqueueExpired(runningThread);

	runningThread = NULL;

	return SUCCESS;
}

// pega a thread em execucao
// decrementa em 10 (pois esta saindo da execucao)
// joga na fila de bloqueadas
// e define seu estamo como tal
int blockRunningThread()
{
	TCB_t* runningThread = getRunningThread();
	if(runningThread == NULL) return ERROR;

	runningThread->credReal -= PRIORITY_DECREMENT;
	runningThread->state = BLOQUEADO;

	list_add(&blocked, runningThread);
	return SUCCESS;
}

// retira a thread da lista de bloqueados
// colocando-a na lista de aptos ativos
// e incrementa seus creditos em 20
// TO-DO: melhorar nome?
int unblockThread(int tid)
{
	TCB_t* thread = list_takeByTID(&blocked, tid);
	if(thread == NULL) return ERROR;

	thread->credReal += UNBLOCK_INCREMENT;
	if(thread->credReal > CREDITS_MAX)
	{
		thread->credReal = CREDITS_MAX;
	}

	runningThread->state = APTO;
	enqueueActive(thread);

	return SUCCESS;
}

// remove uma thread de uma fila de aptos
// procurando em cada uma de suas
// filas de prioridade
TCB_t* apt_takeByTID(AptList* aptList, int tid)
{
	if(aptList == NULL) return NULL;
	TCB_t* threadTaken;
	threadTaken = list_takeByTID(&aptList->highPriorityQueue, tid);
	if(threadTaken != NULL) return threadTaken;

	threadTaken = list_takeByTID(&aptList->mediumPriorityQueue, tid);
	if(threadTaken != NULL) return threadTaken;

	threadTaken = list_takeByTID(&aptList->lowPriorityQueue, tid);
	if(threadTaken != NULL) return threadTaken;

	return NULL;
}

// retorna uma thread sem remove-la da sua
// fila de aptos
TCB_t* apt_findByTID(AptList* aptList, int tid)
{
	if(aptList == NULL) return NULL;
	TCB_t* threadFound;
	threadFound = list_findByTID(&aptList->highPriorityQueue, tid);
	if(threadFound != NULL) return threadFound;

	threadFound = list_findByTID(&aptList->mediumPriorityQueue, tid);
	if(threadFound != NULL) return threadFound;

	threadFound = list_findByTID(&aptList->lowPriorityQueue, tid);
	if(threadFound != NULL) return threadFound;

	return NULL;
}

// recebe um mutex e a thread a ser bloqueada
// decrementa os creditos da thread em 10 (dez) creditos
// e coloca a thread na lista de bloqueados do mutex
int blockThreadForMutex(pimutex_t *mtx, TCB_t* thread)
{
	runningThread->credReal -= PRIORITY_DECREMENT;
	if(runningThread->credReal < 0) runningThread->credReal = 0;

	thread->state = BLOQUEADO;
	if(mtx->first == NULL) mtx->first = thread;

	thread->prev = mtx->last;
	thread->next = NULL;
	mtx->last->next = thread;
	mtx->last = thread;

	return SUCCESS;
}

// recebe um um mutex e retira a primeira thread da fila
// de bloqueados do mutex, incrementando 20 (vinte) creditos
// nela
int unblockMutexThreads(pimutex_t *mtx)
{
	if(mtx == NULL) return ERROR;

	TCB_t* toBeUnlocked = mtx->first;

	mtx->first = toBeUnlocked->next;
	if(mtx->last == toBeUnlocked) mtx->last = NULL;

	toBeUnlocked->state = APTO;
	toBeUnlocked->credReal += UNBLOCK_INCREMENT;
	if(toBeUnlocked->credReal > CREDITS_MAX)
	{
		toBeUnlocked->credReal = CREDITS_MAX;
	}

	enqueueActive(toBeUnlocked);

	return SUCCESS;
}

void printAptosLists()
{
	list_print(aptos_ativos->highPriorityQueue);
	list_print(aptos_ativos->mediumPriorityQueue);
	list_print(aptos_ativos->lowPriorityQueue);
}

AptList* getAptosAtivos()
{
	return aptos_ativos;
}

AptList* getAptosExpirados()
{
	return aptos_expirados;
}

WaitingList* getWaitingList()
{
	return &waitingList;
}