#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "pithread.h"

#include "pidata.h"

#include "list.h"

#define SUCCESS 0
#define ERROR -1

#define HIGH_PRIORITY_CREDITS 80
#define MEDIUM_PRIORITY_CREDITS 40
#define LOW_PRIORITY_CREDITS 0

// usar no scheduler, por enquanto esta sendo
// usado so pra testes
List aptos_ativos = NEW_LIST;
List aptos_expirados = NEW_LIST;


List highPriorityQueue;
List mediumPriorityQueue;
List lowPriorityQueue;

ucontext_t* exit_context;


int picreate (int credCreate, void* (*start)(void*), void *arg)
{

	// aloca o espaco do contexto de saida
	// o contexto de saida contem as informacoes
	// de onde o fluxo de execucao ira seguir,
	// apos o termino da thread 
	if(exit_context == NULL)
	{
		exit_context = (ucontext_t*) malloc(sizeof(ucontext_t));
		if(exit_context == NULL) return -1; // erro de alocacao de memoria

		exit_context->uc_link = NULL;
		exit_context->uc_stack.ss_sp = (char*) malloc(sizeof(SIGSTKSZ));
		
		if(exit_context->uc_stack.ss_sp == NULL) // erro de alocacao de memoria
		{
			free(exit_context); // libera para nao ter mem. leak
			return -1;
		}
		exit_context->uc_stack.ss_size = SIGSTKSZ;
	}



	/**************************************************************/
	printf(" **************************************************** \n");
	printf(" * Aptos Ativos: %i \t\t", aptos_ativos.size);
	printf(" * Aptos Expirados: %i \n", aptos_expirados.size);
	//printf(" **************************************************** \n");
	/**************************************************************/
	//printf(" **************************************************** \n");
	printf(" \t\tCriando nova thread...   \n");
	printf(" **************************************************** \n\n");
	/**************************************************************/


	TCB_t* newThread = (TCB_t*) malloc(sizeof(TCB_t));

	if(newThread == NULL) return -1; // erro de alocacao de memoria

	newThread->tid = highPriorityQueue.size + mediumPriorityQueue.size + lowPriorityQueue.size; 
	// ou
	//newThread->tid = aptos_ativos.size + aptos_expirados.size;
	
	newThread->state = 1;
	newThread->credCreate = credCreate;
	newThread->credReal = credCreate;

	getcontext(&newThread->context);

	printf("\n\n%i\n\n", *((int*) arg));

	printf("\nPointer Address: %p\n",&newThread->context.uc_link);

	//makecontext(&newThread->context, (void (*)(void)) start, 1, (void*) arg);



	// adiciona a thread a sua respectiva
	// fila, dependendo dos seus creditos
	// de criacao
	if(credCreate > HIGH_PRIORITY_CREDITS)
	{
		list_append(&highPriorityQueue, newThread);
		printf("Thread adicionada em: ");
		printf("\t High Priority Queue\n");
		list_print(highPriorityQueue);
	}
	else
	{
		if(credCreate > MEDIUM_PRIORITY_CREDITS)
		{
			list_append(&mediumPriorityQueue, newThread);
			printf("Thread adicionada em: ");
			printf("\t Medium Priority Queue\n");
			list_print(mediumPriorityQueue);
		}
		else
		{
			list_append(&lowPriorityQueue, newThread);
			printf("Thread adicionada em: ");
			printf("\t Low Priority Queue\n");
			list_print(lowPriorityQueue);
			printf("\n");
		}
	}

	//list_add(&aptos_ativos, newThread);

	// /**************************************************************/
	// printf(" **************************************************** \n");
	// printf(" * Aptos Ativos: %i \t\t", aptos_ativos.size);
	// printf(" * Aptos Expirados: %i \n", aptos_expirados.size);
	// printf(" **************************************************** \n");
	// /**************************************************************/

	// /**************************************************************/
	// printf("\n");
	// printf(" * Thread TID: %i \n", newThread->tid);
	// printf(" * Thread state: %i \n", newThread->state);
	// printf(" * Thread credits: %i \n", newThread->credCreate);
	// printf(" * Thread dynamic credits: %i \n", newThread->credReal);

	// if(newThread->prev != NULL) printf(" * Prev Thread TID: %i \n", newThread->prev->tid);
	// else printf(" * Prev Thread TID: NULL \n"); 

	// if(newThread->next != NULL) printf(" * Next Thread TID: %i \n", newThread->next->tid);
	// else printf(" * Next Thread TID: NULL \n");	

	// printf("\n");
	// /**************************************************************/	

	//free(newThread);

	//printf("\t Aptos Ativos\n");
	//list_print(aptos_ativos);

	return SUCCESS;
}

int piyield(void)
{
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