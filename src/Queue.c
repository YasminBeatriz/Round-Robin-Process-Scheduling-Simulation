#include "Queue.h"
#include <stdlib.h>

/* Standard queue implemented using Linked List */

Queue* initQueue()
{
	return initLista();
}

void enQueue(Queue *queue, void* value)
{
	//insertBeginning(queue, value, -1); //Every element in a standard queue has the same priority
	insertEnd(queue, value, PROCESS, -1);
	//insertByPriority(queue, value, PROCESS, 0);
}

void *deQueue(Queue *queue)
{
	void *removed = removeBeginning(queue);
	return removed;
}

int EmptyQueue(Queue *queue)
{
	return isEmpty(queue);
}

void printQueue(Queue *queue)
{
	printList(queue);
}

long int getSize(Queue *queue)
{
	return size(queue);
}

void releaseQueue(Queue *queue)
{
	releaseList(queue);
}