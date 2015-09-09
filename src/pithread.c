#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ucontext.h>
#include "pithread.h"

#include "pidata.h"

#include "list.h"
#include "scheduler.h"

#define SUCCESS 0
#define ERROR -1

// no caso, o contexto de saida sera usado para 
// chamar o escalonador, indicando que a [thread desejada]
// terminou e deve ser removida da lista de threads aptas
ucontext_t* exit_context;


int picreate (int credCreate, void* (*start)(void*), void *arg)
{

	// aloca o espaco do contexto de saida
	// o contexto de saida contem as informacoes
	// de onde o fluxo de execucao ira seguir,
	// apos o termino da thread

	// no caso, o contexto de saida sera usado para 
	// chamar o escalonador, indicando que a [thread desejada]
	// terminou e deve ser removida da lista de threads aptas
	if(exit_context == NULL)
	{
		exit_context = (ucontext_t*) malloc(sizeof(ucontext_t));
		if(exit_context == NULL) return ERROR; // erro de alocacao de memoria

		exit_context->uc_link = NULL;
		exit_context->uc_stack.ss_sp = (char*) malloc(sizeof(SIGSTKSZ));
		
		if(exit_context->uc_stack.ss_sp == NULL) // erro de alocacao de memoria
		{
			printf("Erro ao alocar a pilha do contexto de saida...\n");
			free(exit_context); // libera para nao ter mem. leak
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
	}

	/**************************************************************/
	printf(" **************************************************** \n");
	printf(" \t\tCriando nova thread...   \n");
	printf(" **************************************************** \n\n");
	/**************************************************************/


	TCB_t* newThread = (TCB_t*) malloc(sizeof(TCB_t));
	if(newThread == NULL) return ERROR; // erro de alocacao de memoria

	newThread->tid = getNewTID();	
	newThread->state = APTO;
	newThread->credCreate = credCreate;
	newThread->credReal = credCreate;

	// aloca a pilha nova da thread em execucao
	// e define o contexto de saida da thread para
	// o contexto de saida geral
	newThread->context.uc_link = exit_context;
	newThread->context.uc_stack.ss_sp = (char*) malloc(sizeof(SIGSTKSZ));
	if(newThread->context.uc_stack.ss_sp == NULL)
	{
		printf("Erro ao alocar a pilha do contexto da nova thread...\n");
		free(newThread);
		free(exit_context);
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

	enqueueActive(newThread);
	printThread(newThread);

	return newThread->tid;
}

int piyield(void)
{
	TCB_t* runningThread = getRunningThread();
	TCB_t* nextThread;

	if(runningThread == NULL)
		return ERROR;
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

	setcontext(&nextThread->context);

	return SUCCESS;
}

int piwait(int tid)
{
	return SUCCESS;
}

int pimutex_init(pimutex_t *mtx)
{
	return SUCCESS;
}

int pilock (pimutex_t *mtx)
{
	return SUCCESS;
}

int piunlock (pimutex_t *mtx)
{
	return SUCCESS;
}