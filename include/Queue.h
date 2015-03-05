#include "LinkedList.h"

#define TRUE	1
#define	FALSE	0

#ifndef QUEUE_H
#define QUEUE_H 
	typedef Lista Queue;
#endif

/* Standard queue implemented using Linked List */

/* FUNCTION PROTOTYPES */

Queue* initQueue();

void enQueue(Queue *queue, void* value);

void *deQueue(Queue *queue);

int EmptyQueue(Queue *queue);

void printQueue(Queue *queue);

void releaseQueue(Queue *queue);

long int getSize(Queue *queue);
