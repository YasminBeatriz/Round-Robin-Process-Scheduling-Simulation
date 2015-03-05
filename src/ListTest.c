#include <stdio.h>
#include "Queue.h"


#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include "Queue.h"

double beginning, now;
int probability[100];
int num_processes = 0;

enum {CPU_BOUND, IO_BOUND};

enum {NEW_PROCESS, READY_CPU, QUANTUM_EXPIRATION, IO_INTERRUPT, FINISHED_IO, PROCESS_FINISHED};

typedef struct proc
{
	long int id;
	double time_created;
	int bound;
	//spent time in Ready Queue, CPU and I/O
	double currentReadyQueueTime;
	double currentCPUTime;
	double currentIOTime;
	double remainingCPUBurst;
	double totalCPUsofar;
	double totalCPUTimeRemaining;
} Process;

typedef struct ev
{
	int type;
	double eventTime;
} Event;

Process *newProcess();

double randomizeUniformly(int high, int low);

double randomizeExponentialy(int parameter);

double getTimeElapsed(double b);

int main()
{

	Queue *q;

	q = initQueue();
	int i, a;

	for(int i = 0; i < 10; i++)
	{
		Process *p = newProcess();
		printf("enQueueing process id = %ld\nTime created = %f \tType = %d\n\n", p->id, p->time_created, p->bound);
		enQueue(q, p);
	}

	printf("Fila: \n");

	i = 0;
	while(i < 6)
	{
		Process *r = deQueue(q);
		printf("Process id = %ld\n", r->id);
		i++;
	}

	a = 0;
	while(a < 6)
	{
		Process *p = newProcess();
		printf("enQueueing process id = %ld\nTime created = %f \tType = %d\n\n", p->id, p->time_created, p->bound);
		enQueue(q, p);
		a++;
	}

	i = 0;
	while(!isEmpty(q))
	{
		Process *r = deQueue(q);
		printf("Process id = %ld\n", r->id);
		i++;
	}

	return 0;
}

Process *newProcess()
{
	Process *p = (Process *) malloc (sizeof(Process));
	p->id = num_processes;
	num_processes += 2;
	p->time_created = getTimeElapsed(beginning);
	p->totalCPUTimeRemaining = 0.900000; //randomizeExponentialy(avgProcessLength);
	p->currentReadyQueueTime = 0.000000;
	p->currentIOTime = 0.000000;
	p->totalCPUsofar = 0.000000;

	int index = randomizeUniformly(0, 99);	
	p->bound = probability[index];
	
	if(p->bound == CPU_BOUND)
		p->currentCPUTime = randomizeUniformly(10000, 20000);
	else
		p->currentCPUTime = randomizeUniformly(2000, 4000);

	return p;
}

double randomizeExponentialy(int parameter)
{
	double x;
	
	srand((unsigned)time(NULL));
	x = -parameter * log ((float)rand() / RAND_MAX );

/*	x *= 10000;
	x = (long int) x;
	x /= 10000; */

	return x;
}

double randomizeUniformly(int high, int low)
{
	double x = 0.0;
	unsigned int range;
	int aux;

	if(low > high)
	{
		aux = low;
		low = high;
		high = aux;
	}

	range = (unsigned int)high - (unsigned int) low;

	srand((unsigned)time(NULL));
	x = (double) rand() / RAND_MAX;
	x = x * (range) + low;

/*
	x *= 10000;
	x = (long int) x;
	x /= 10000; */

	return x;
}

double getTimeElapsed(double b)
{
    struct timeval tm;
    gettimeofday( &tm, NULL);
    double t = (double) tm.tv_sec + (double)tm.tv_usec / 1000000.0;

    t = t - b;

    //truncating t
    t *= 10000;
	t = (long int) t;
	t /= 10000;

    return t;
}
