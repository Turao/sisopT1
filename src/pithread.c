#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ucontext.h>
#include "pithread.h"

#include "pidata.h"

#include "list.h"
#include "waitingList.h"
#include "scheduler.h"

#define TRUE 1
#define FALSE 0

#define SUCCESS 0
#define ERROR -1

#define MAX_CRED 100
#define MIN_CRED 1

#define MAIN_THREAD_CREDITS 100

// no caso, o contexto de saida sera usado para 
// chamar o escalonador, indicando que a [thread desejada]
// terminou e deve ser removida da lista de threads aptas
ucontext_t* exit_context;



int mainTCBCreated = FALSE;


int allocExitContext()
{
	exit_context = (ucontext_t*) malloc(sizeof(ucontext_t));
	if(exit_context == NULL) return ERROR; // erro de alocacao de memoria

	exit_context->uc_link = NULL;
	exit_context->uc_stack.ss_sp = (char*) malloc(sizeof(SIGSTKSZ));
		
	if(exit_context->uc_stack.ss_sp == NULL) // erro de alocacao de memoria
	{
		// printf("Erro ao alocar a pilha do contexto de saida...\n");
		//free(exit_context); // libera para nao ter mem. leak
		return ERROR;
	}
	exit_context->uc_stack.ss_size = SIGSTKSZ;
	// preenche outros campos necessarios que nao sao
	// muito importantes para o escalonamento
	// a partir do contexto atual
	getcontext(exit_context);

	// quando chamado (ou seja, ao final da execucao)
	// de cada thread, o contexto de saida
	// deve chamar a funcao de termino de thread,
	// do escalonador
	makecontext(exit_context, (void (*)(void)) terminateThread, 0, NULL);
	return SUCCESS;
}


/* FUNCAO QUE NAO TEM NO HEADER
*  CRIA UMA TCB PARA A MAIN
*  E A COLOCA NA FILA DE APTOS ATIVOS
*  COM PRIORIDADE MAXIMA
*/
int createMainTCB()
{
	TCB_t* mainThread = (TCB_t*) malloc(sizeof(TCB_t));
	if (mainThread == NULL) return ERROR;
	// printf(" * Criando main thread...\n");
    
	mainThread->state = EXECUCAO;
	mainThread->credCreate = MAIN_THREAD_CREDITS;
	mainThread->credReal = mainThread->credCreate;
	mainThread->tid = 0;
	mainThread->next = NULL;
	mainThread->prev = NULL;

	mainThread->context.uc_link = NULL;
	mainThread->context.uc_stack.ss_sp = (char*) malloc(sizeof(SIGSTKSZ));
	if(mainThread->context.uc_stack.ss_sp == NULL)
	{
		// printf("Erro ao alocar a pilha do contexto da nova thread...\n");
		free(mainThread);
		return ERROR;
	}
	mainThread->context.uc_stack.ss_size = SIGSTKSZ;

 	getcontext(&mainThread->context);
	mainTCBCreated = TRUE;
	setRunningThread(mainThread);    

	return SUCCESS;
}






int picreate (int credCreate, void* (*start)(void*), void *arg)
{
	if(mainTCBCreated == FALSE) createMainTCB();

	// aloca o espaco do contexto de saida
	// o contexto de saida contem as informacoes
	// de onde o fluxo de execucao ira seguir,
	// apos o termino da thread

	// no caso, o contexto de saida sera usado para 
	// chamar o escalonador, indicando que a [thread desejada]
	// terminou e deve ser removida da lista de threads aptas
	if(exit_context == NULL)
	{
		if(allocExitContext() == ERROR) return ERROR;
	}

	/**************************************************************/
	// printf(" **************************************************** \n");
	// printf(" \t\tCriando nova thread...   \n");
	// printf(" **************************************************** \n\n");
	/**************************************************************/

	TCB_t* newThread = (TCB_t*) malloc(sizeof(TCB_t));
	if(newThread == NULL) return ERROR; // erro de alocacao de memoria
	
	if(MIN_CRED <= credCreate && credCreate >= MAX_CRED)
	{
		newThread->credCreate = credCreate;
		newThread->credReal = credCreate;
	}
	else
	{
		free(newThread);
		return ERROR;
	}

	newThread->tid = getNewTID();	
	newThread->state = APTO;
	// aloca a pilha nova da thread em execucao
	// e define o contexto de saida da thread para
	// o contexto de saida geral
	newThread->context.uc_link = exit_context;
	newThread->context.uc_stack.ss_sp = (char*) malloc(sizeof(SIGSTKSZ));
	if(newThread->context.uc_stack.ss_sp == NULL)
	{
		// printf("Erro ao alocar a pilha do contexto da nova thread...\n");
		free(newThread);
		return ERROR;
	}
	newThread->context.uc_stack.ss_size = SIGSTKSZ;
	
	// preenche outros campos necessarios que nao sao
	// muito importantes para o escalonamento
	// a partir do contexto atual
	getcontext(&newThread->context);

	// faz a ligacao entre a funcao que sera chamada
	// e o contexto que sera utilizado nela
	makecontext(&newThread->context, (void (*)(void)) start, 1, (void*) arg);

	if(credCreate <= 0) enqueueExpired(newThread);
	else enqueueActive(newThread);

	// printThread(newThread);

	return newThread->tid;
}

int piyield(void)
{
	if(mainTCBCreated == FALSE) createMainTCB();

	TCB_t* runningThread = getRunningThread();
	// printThread(runningThread);
	TCB_t* nextThread;

	if(runningThread == NULL) return ERROR;
	else
	{
		runningThread->state = APTO;
		if(runningThread->credReal >= PRIORITY_DECREMENT)
		{
			deactivateRunningThread();
		}
		else if(runningThread->credReal < PRIORITY_DECREMENT)
		{
			expireRunningThread();
		}
		
		// chama o escalonador para pegar a proxima thread
		nextThread = getNextThread();
	}

	if(nextThread == NULL)
	{
		// printf(" Erro ao escolher proxima thread! \n");
		return ERROR;
	}
	else
	{
		// printf("yield TID: %i \n", runningThread->tid);
		runThread(runningThread, nextThread);
		return SUCCESS;
	}
}

int piwait(int tid)
{
	if(mainTCBCreated == FALSE) createMainTCB();

	// thread a ser esperada existe?
	// para isso, procuramos a thread na lista de aptos ativos
	AptList* aptList = getAptosAtivos();
	TCB_t* toBeWaited = apt_findByTID(aptList, tid);
	if(toBeWaited == NULL)
	{
		// se nao encontrarmos nada na fila de aptos ativos,
		// procuramos na fila de aptos expirados
		// e se nao encontrarmos novamente,
		// eh porque ela nao exite
		aptList = getAptosExpirados();
		toBeWaited = apt_findByTID(aptList, tid);
		if(toBeWaited == NULL) return ERROR;
	}

	WaitingList* waitingList = getWaitingList();

	// thread TID ja tem outra thread a esperando?
	if(waitingList_isBeingWaited(waitingList, toBeWaited) != NULL) return ERROR;

	// senao, adiciona a thread a lista de threads em espera
	WaitingInfo* waitingInfo = (WaitingInfo*) malloc(sizeof(WaitingInfo));
	if(waitingInfo == NULL) return ERROR;
	TCB_t* runningThread = getRunningThread();
	waitingInfo->waiting = runningThread;
	waitingInfo->beingWaited = toBeWaited;
	waitingInfo->next = NULL;
	waitingInfo->prev = NULL;

	blockRunningThread();
	waitingList_append(waitingList, waitingInfo);

	// chama o escalonador para pegar a proxima thread
	TCB_t* nextThread = getNextThread();
	if(nextThread == NULL)
	{
		// printf(" Erro ao escolher proxima thread! \n");
		return ERROR;
	}
	else
	{
		// printf("Thread TID: %i \t waiting for thread TID: %i \n",
		// waitingInfo->waiting->tid, waitingInfo->beingWaited->tid);
		runThread(runningThread, nextThread);
		return SUCCESS;
	}

	return ERROR;
}

int pimutex_init(pimutex_t *mtx)
{
	if(mainTCBCreated == FALSE) createMainTCB();

	mtx->flag = LIVRE;
	mtx->first = NULL;
	mtx->last = NULL;

	return SUCCESS;
}

int pilock(pimutex_t *mtx)
{
	if(mtx == NULL) return ERROR;

	if(mainTCBCreated == FALSE) createMainTCB();

	TCB_t* runningThread = getRunningThread();
	TCB_t* nextThread = NULL;

	if(runningThread == NULL) return ERROR;
	else if(mtx->flag == LIVRE)
	{
		mtx->flag = OCUPADO;
		return SUCCESS;
	}
	else if(mtx->flag == OCUPADO)
	{
		runningThread = getRunningThread();
		nextThread = getNextThread();

		blockThreadForMutex(mtx, runningThread);

		if(nextThread == NULL)
		{
			// printf(" Erro ao escolher proxima thread! \n");
			return ERROR;
		}
		else
		{
			runThread(runningThread, nextThread);
			return SUCCESS;
		}
	}
	return ERROR;
}

int piunlock(pimutex_t *mtx)
{
	if(mtx == NULL) return ERROR;
	if(mainTCBCreated == FALSE) createMainTCB();

	mtx->flag = LIVRE;
	unblockMutexThreads(mtx);

	return SUCCESS;
}
