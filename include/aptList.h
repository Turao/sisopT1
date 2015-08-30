#ifndef __APTLIST__
#define __APTLIST__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pidata.h"

#define NEW_APT_LIST {NULL, NULL, 0}

typedef struct APT {
	TCB_t *first, *last; // ponteiros para primeiro e ultimo TCBs
	int size;
} AptList;


void apt_init(AptList* list);
bool apt_isEmpty(AptList list);
void apt_append(AptList* list, TCB_t* tcb);
void apt_prepend(AptList* list, TCB_t* tcb);
void apt_add(AptList* list, TCB_t* tcb);
TCB_t* apt_at(AptList* list, int i);
TCB_t* apt_popBack(AptList* list);
TCB_t* apt_popFront(AptList* list);

void apt_print(AptList list);

#endif // __APTLIST__