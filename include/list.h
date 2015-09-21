#ifndef __LIST__
#define __LIST__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pidata.h"

#define NEW_LIST {NULL, NULL, 0}

typedef struct List {
	TCB_t *first, *last; // ponteiros para primeiro e ultimo TCBs
	int size;
} List;


/* Inicia os atributos da lista
*  com ponteiros NULL e tamanho 0
*/
void list_init(List* list);


/* Retorna True caso seja uma lista vazia,
*  False caso contrario
*/
bool list_isEmpty(List* list);


/* Adiciona uma tcb ao final da lista */
void list_append(List* list, TCB_t* tcb);


/*Adiciona uma tcb ao inicio da lista */
void list_prepend(List* list, TCB_t* tcb);


/* Adiciona uma tcb a lista seguindo uma ordenacao 
*  decrescente de creditos dinamicos, seguindo uma
*  politica de FIFO em caso de duas ou mais tcbs
*  com creditos dinamicos iguais
*/
void list_add(List* list, TCB_t* tcb);


/* Retorna a tcb na posicao p 
*  Retorna NULL caso a lista seja vazia
*  ou nao exista elemento na posicao p
*/
TCB_t* list_at(List* list, int p);


/* Remove e retorna o ultimo elemento da lista
*  Retorna NULL caso a lista esteja vazia 
*/
TCB_t* list_popBack(List* list);


/* Remove e retorna o primeiro elemento da lista
*  Retorna NULL caso a lista esteja vazia 
*/
TCB_t* list_popFront(List* list);


/* Imprime o conteudo de uma lista */
void list_print(List list);


/* Encontra a tcb na lista passada
*  Retorna NULL caso nao encontre
*/
TCB_t* list_findByTID(List* list, int tid);


/* Encontra a tcb na lista passada E remove ela da lista
*  Retorna NULL caso nao encontre
*/
TCB_t* list_takeByTID(List* list, int tid);

#endif // __LIST__