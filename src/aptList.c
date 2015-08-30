#include <stdio.h>
#include <stdlib.h> 
#include "aptList.h"
#include "pidata.h"

/* Lista duplamente encadeada (nao circular) */


/* Inicializa os atributos da estrutura */
void apt_init(AptList* list)
{
	list->first = NULL;
	list->last = NULL;
	list->size = 0;
}

/* Retorna um booleano dizendo se a lista passada é vazia */
bool apt_isEmpty(AptList list)
{
	if(list.size == 0) return true;
	else return false;
}

/* Insere um elemento no final da lista */
void apt_append(AptList* list, TCB_t* tcb)
{
	if(apt_isEmpty(*list))
	{
		list->first = tcb;
		list->last = tcb;
		list->size = 1;

		tcb->prev = NULL;
		tcb->next = NULL;
	}
	else
	{
		TCB_t* oldLast = list->last;
		list->last = tcb;
		oldLast->next = tcb;
		list->size += 1;
		tcb->prev = oldLast;
		tcb->next = NULL;
	}
}

/* Insere um elemento no inicio da lista */
void apt_prepend(AptList* list, TCB_t* tcb)
{
	if(apt_isEmpty(*list))
	{
		list->first = tcb;
		list->last = tcb;
		list->size = 1;

		tcb->prev = NULL;
		tcb->next = NULL;
	}
	else
	{
		TCB_t* oldFirst = list->first;
		list->first = tcb;
		oldFirst->prev = tcb;
		list->size += 1;
		tcb->prev = NULL;
		tcb->next = oldFirst;
	}
}

/* Adiciona uma tcb a lista, de acordo com seu valor */
/* de creditos definidos durante a criacao (credCreate) */
void apt_add(AptList* list, TCB_t* tcb)
{
	int credCreate = tcb->credCreate;

	if(apt_isEmpty(*list)) apt_append(list, tcb);
	else
	{
		if(list->last->credCreate <= credCreate)
		{
			apt_append(list, tcb);
		}
		else
		{
			int i = 0;
			TCB_t* currentTCB = list->first;
			TCB_t* previousTCB = NULL;
			for(; i < list->size; i++)
			{
				// se os creditos da tcb em questao forem menores
				// do que os creditos da tcb que queremos inserir,
				// passamos para a proxima da lista
				if(currentTCB->credCreate < credCreate)
				{
					previousTCB = currentTCB;
					currentTCB = currentTCB->next;	
				}
				// caso os creditos da tcb em questao forem MAIORES ou IGUAIS,
				// significa que a tcb que desejamos inserir deve ser colocada atras
				// da tcb em questao
				else
				{
					if(previousTCB) previousTCB->next = tcb;
					if(currentTCB) currentTCB->prev = tcb;

					tcb->next = currentTCB;
					tcb->prev = previousTCB;
					// caso a tcb seja inserida na primeira posicao,
					// devemos atualizar o atributo first de AptList
					if(currentTCB == list->first) list->first = tcb;

					list->size += 1;
					break; // e paramos o for
				}
			}
		}
	}
}

/* Retorna o elemento da posicao p na lista */
/* Retorna NULL caso nao exista */
TCB_t* apt_at(AptList* list, int p)
{
	// por motivos de conveniencia,
	// como estamos acostumados a usar
	// o indice 0 para o primeiro elemento,
	// faco p+1 no condicional...
	if(list->size < (p+1))
	{
		return NULL;
	}
	else
	{
		TCB_t* tcb = list->first;
		int i = 0;
		for(; i < p; i++)
		{
			tcb = tcb->next;
		}
		return tcb;
	}
}

/* Remove o ultimo elemento da lista */
/* Retorna NULL caso a lista esteja vazia */
TCB_t* apt_popBack(AptList* list)
{
	if(apt_isEmpty(*list))
	{
		return NULL;
	}

	if(list->size == 1)
	{
		TCB_t* tcb = list->last;
		list->first = NULL;
		list->last = NULL;
		list->size = 0;
		return tcb;
	}

	else
	{
		TCB_t* tcb = list->last;
		TCB_t* newLast = tcb->prev;
		list->last = newLast;
		newLast->next = NULL;
		list->size -= 1;
		//nao lembro se tem algo mais, deu branco aqui
		// TO-DO: revisar
		return tcb;
	}
}

/* Remove o primeiro elemento da lista */
TCB_t* apt_popFront(AptList* list)
{
	if(apt_isEmpty(*list))
	{
		return NULL;
	}

	if(list->size == 1)
	{
		TCB_t* tcb = list->first;
		list->first = NULL;
		list->last = NULL;
		list->size = 0;
		return tcb;
	}

	else
	{
		TCB_t* tcb = list->first;
		TCB_t* newFirst = tcb->next;
		list->first = newFirst;
		newFirst->prev = NULL;
		list->size -= 1;
		//nao lembro se tem algo mais, deu branco aqui
		// TO-DO: revisar
		return tcb;
	}
}

void apt_print(AptList list)
{
	printf("Lista de Aptos: \n");
	int i = 0;
	TCB_t* currentTCB = list.first;
	for (; i < list.size; i++)
	{
		printf("\t Thread TID: %3i \t CredCreate: %3i \t CredReal: %3i \n",
		 currentTCB->tid, currentTCB->credCreate, currentTCB->credReal);

		currentTCB = currentTCB->next;
	}
}