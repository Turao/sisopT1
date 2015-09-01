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


void list_init(List* list);
bool list_isEmpty(List list);
void list_append(List* list, TCB_t* tcb);
void list_prepend(List* list, TCB_t* tcb);
void list_add(List* list, TCB_t* tcb);
TCB_t* list_at(List* list, int i);
TCB_t* list_popBack(List* list);
TCB_t* list_popFront(List* list);

void list_print(List list);

#endif // __LIST__