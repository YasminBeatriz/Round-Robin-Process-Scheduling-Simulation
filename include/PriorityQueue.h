#include "LinkedList.h"

#ifndef PRIORITY_H 
#define PRIORITY_H
	typedef Lista PriorityQueue;
#endif

/* Priority queue implemented using Linked List */

/* FUNCTION PROTOTYPES */

PriorityQueue* initPriorityQueue();

void Priority_enQueue(PriorityQueue *pQueue, void *value, element pr);

void *Priority_deQueue(PriorityQueue *pQueue);

int EmptyPriorityQueue(PriorityQueue *pQueue);

void printPQueue(PriorityQueue *pQueue);

void releasePriorityQueue(PriorityQueue *pQueue);