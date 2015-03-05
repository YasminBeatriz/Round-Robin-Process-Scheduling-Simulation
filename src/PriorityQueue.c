#include "PriorityQueue.h"
#include <stdio.h>
#include <stdlib.h>

/* Priority queue implemented using Linked List */

PriorityQueue* initPriorityQueue()
{
	return initLista();
}

void Priority_enQueue(PriorityQueue *pQueue, void *value, element pr)
{
	insertByPriority(pQueue, value, EVENT, pr); //Every element in a standard queue has the same priority
}

void *Priority_deQueue(PriorityQueue *pQueue)
{
	void *removed = removeBeginning(pQueue);
	return removed;
}

int EmptyPriorityQueue(PriorityQueue *pQueue)
{
	return isEmpty(pQueue);
}

void printPQueue(PriorityQueue *pQueue)
{
	printList(pQueue);
}

void releasePriorityQueue(PriorityQueue *pQueue)
{
	releaseList(pQueue);
}