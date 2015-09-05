#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#define HIGH_PRIORITY_CREDITS 80
#define MEDIUM_PRIORITY_CREDITS 40
#define LOW_PRIORITY_CREDITS 0

#define SUCCESS 0
#define ERROR -1

// usar no scheduler, por enquanto esta sendo
// usado so pra testes
List aptos_ativos = NEW_LIST;
List aptos_expirados = NEW_LIST;

List highPriorityQueue;
List mediumPriorityQueue;
List lowPriorityQueue;

TCB_t* runningThread;


// apos o termino da thread, essa
// funcao eh chamada,
// nao sei o que deve ser feito por enquanto
// so que devemos chamar o escalonador no final
// pra que ele escolha a proxima thread
void* terminateThread()
{
	//int* actualTID = param;

	printf("\n\t- A thread terminou. -\n");

	return NULL;
}


// adiciona uma thread a sua fila correspondente
// baseado em seus creditos de criacao
void enqueue(TCB_t* thread)
{
	// adiciona a thread a sua respectiva
	// fila, dependendo dos seus creditos
	// de criacao
	printf("Thread adicionada em: ");
	if(thread->credCreate > HIGH_PRIORITY_CREDITS)
	{
		printf("\t High Priority Queue\n");
		list_append(&highPriorityQueue, thread);
		// list_print(highPriorityQueue);
	}
	else
	{
		if(thread->credCreate > MEDIUM_PRIORITY_CREDITS)
		{
			printf("\t Medium Priority Queue\n");
			list_append(&mediumPriorityQueue, thread);
			// list_print(mediumPriorityQueue);
		}
		else
		{
			printf("\t Low Priority Queue\n");
			list_append(&lowPriorityQueue, thread);
			// list_print(lowPriorityQueue);
			printf("\n");
		}
	}
}


// usado para receber um valor valido de TID
int getNewTID()
{
	return highPriorityQueue.size + mediumPriorityQueue.size + lowPriorityQueue.size;
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
	printf(" **************************************************** \n");
	printf(" * Aptos Ativos: %i \t\t", aptos_ativos.size);
	printf(" * Aptos Expirados: %i \n", aptos_expirados.size);
	printf(" **************************************************** \n");

	//printf("\t Aptos Ativos\n");
	//list_print(aptos_ativos);
}

// imprime o conteudo de uma thread
void printThread(TCB_t* thread)
{
	printf("\n");
	printf(" * Thread TID: %i \n", thread->tid);
	printf(" * Thread state: %i \n", thread->state);
	printf(" * Thread credits: %i \n", thread->credCreate);
	printf(" * Thread dynamic credits: %i \n", thread->credReal);

	if(thread->prev != NULL) printf(" * Prev Thread TID: %i \n", thread->prev->tid);
	else printf(" * Prev Thread TID: NULL \n"); 

	if(thread->next != NULL) printf(" * Next Thread TID: %i \n", thread->next->tid);
	else printf(" * Next Thread TID: NULL \n");	

	printf("\n");

}