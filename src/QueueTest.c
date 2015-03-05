#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
#include "PriorityQueue.h"

typedef struct pro
{
	int num_process;
} process;

int main()
{
	PriorityQueue *pQ;
	Queue *q;
	process *r;
	
	pQ = initPriorityQueue();
	q = initQueue();

	printf("TESTING STANDARD QUEUE\n\n");

	printf("Printing queue:\n");
	printQueue(q);
	
	process *p = (process *) malloc (sizeof(process));
	p->num_process = 9;
	enQueue(pQ, p);

	r = deQueue(pQ);
	printf("Num process = %d\n", r->num_process);

	return 0;
}