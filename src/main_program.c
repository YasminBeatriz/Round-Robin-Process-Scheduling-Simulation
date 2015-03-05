/* 	CPU Scheduling simulation 
	Author: Yasmin Beatriz Alves da Silva */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include "Queue.h"
#include "PriorityQueue.h"

#define MAX_LINE_LENGTH	60

enum {CPU_BOUND, IO_BOUND};

enum {NEW_PROCESS, READY_CPU, QUANTUM_EXPIRATION, IO_INTERRUPT, FINISHED_IO, PROCESS_FINISHED};

/* PROCESS CLASS */
typedef struct proc
{
	int id;
	double time_created;
	int bound;
	//spent time in Ready Queue, CPU and I/O
	double currentReadyQueueTime;
	double currentCPUTime;
	double currentIOTime;
	double remainingCPUBurst;
	double totalCPUsofar;
	double totalCPUTimeRemaining;
	int interrupts;
} Process;

/* EVENT CLASS */
typedef struct ev
{
	int type;
	double eventTime;
} Event;

//variables from user (global)
int total_simulation;
double quantum;
int context_switch;
int avgProcessLength;
int avgCreationTime;
int IOBoundPct;
int avgIOserviceTime;

int probability[100]; //to get random values for CPU and I/O bound processes
int num_processes = 0;

double beginning, now; //to keep track of time


/*************** FUNCTION PROTOTYPES **************/

void getParameters(int *total_seconds, double *quantum, int *context_switch, int *avgProcessLength, int *avgCreationTime, 
	int *IOBoundPct, int *avgIOserviceTime);

double getTimeElapsed(double b);

double randomizeUniformly(int high, int low);

double randomizeExponentialy(int parameter);

double getSeconds(double us);

Process *newProcess();

int main(int argc, char const *argv[])
{
	int bound_type;

	//Creating and initializing Queue and Priority Queue
	Queue *ReadyQueue = initQueue();
	Queue *IOQueue = initQueue();
	PriorityQueue *EventQueue = initPriorityQueue();

	beginning = getTimeElapsed(0.00000);
	now = getTimeElapsed(beginning);

	double avgCPUtime = 0, avgCPUTime_forIO = 0, avgCPUTime_forCPU = 0;
	double totalCPUutilization = 0, avgCPUutilization_forFinished = 0, avgCPUtime_forFinished_CPU = 0, avgCPUtime_forFinished_IO = 0;
	double avgReadyTimeAll = 0, avgReadyTime_forCPU = 0, avgReadyTime_forIO = 0;
	double avgIOinterrupts_forCPU = 0, avgIOinterrupts_forIO = 0;
	double turnaroundAll = 0, turnaroundCPU = 0, turnaroundIO = 0;
	long int contextSwitchCount = 0, processes_finished = 0;
	long int totalIOProcessCompleted = 0, totalCPUProcessCompleted = 0;
;

	//readyTime determines the average waiting time of a process in the Ready Queue
	//timeIO is the IO service time for each process
	//timeCPU is the CPU utilization by each process

	double readyTime, timeIO;
	printf("Queues initialized at time: %.6f\n", now);

	Queue *CPU = initQueue();

	getParameters(&total_simulation, &quantum, &context_switch, &avgProcessLength, &avgCreationTime, &IOBoundPct, &avgIOserviceTime);
	
	/* 	Initializing probability[] with IO-bound and CPU-bound values following the percentage IOBoundPct
		probability[] is an array that provides a percentage of IO-bound processes based on user input 
		every time that I need to random the bound of a process I will pick up a random number from this array */

	bound_type = IO_BOUND;

	for(int i = 0; i < 100; i++)
	{	
		probability[i] = bound_type;
		if(i > IOBoundPct)
			bound_type = CPU_BOUND;
	}
	//printf("\n");

	/* Starting simulation with a first event: NEW PROCESS */
	Event *e = (Event *) malloc (sizeof(Event));
	e->type = NEW_PROCESS;
	e->eventTime = getTimeElapsed(beginning);
	Priority_enQueue(EventQueue, e, e->eventTime);

	Event *r = (Event *) malloc (sizeof(Event));

	while(now < total_simulation)
	{
		if(!EmptyPriorityQueue(EventQueue))
		{
			r = Priority_deQueue(EventQueue);

			now = r->eventTime;

			switch(r->type)
			{
				case NEW_PROCESS:
				{
					Process *p = newProcess();
					printf("\n\n***** NEW PROCESS ***** | ");
					printf("\tTime: %f", now);
					printf(" . PID = %d ", p->id);
					printf(" . Bound: ");
					if(p->bound == CPU_BOUND)
					{
						printf("CPU-bound;");
						avgCPUTime_forCPU += p->currentCPUTime;
					}	
					else
					{
						printf("I/O-bound;");
						avgCPUTime_forIO += p->currentCPUTime;
					}
							
					printf(" . Total CPU burst: %f", getSeconds(p->totalCPUTimeRemaining));
					printf(" . Current CPU burst: %f", getSeconds(p->currentCPUTime));
					enQueue(ReadyQueue, p);
					readyTime = getTimeElapsed(0.000000);

					//Keeping track of average CPU time 
					avgCPUtime += p->currentCPUTime;
					totalCPUutilization += p->totalCPUTimeRemaining;


					if(EmptyQueue(CPU))
					{
						Event *ready_cpu = (Event *) malloc(sizeof(Event));
						ready_cpu->type = READY_CPU;
						ready_cpu->eventTime = now + getTimeElapsed(beginning);
						Priority_enQueue(EventQueue, ready_cpu, ready_cpu->eventTime);
					}

					Event *newProcess = (Event *) malloc (sizeof(Event));
					newProcess->type = NEW_PROCESS;
					newProcess->eventTime = now + getSeconds(randomizeExponentialy(avgCreationTime));

					printf(" . Next process: %f\n", newProcess->eventTime);
					Priority_enQueue(EventQueue, newProcess, newProcess->eventTime);

					break;
				}

				case READY_CPU:
				{
					printf("\n***** NEW READY-CPU ***** | ");
					Process *currentProcess;
					//printf("\tSize ANTES deQueue EM READY CPU. %ld", getSize(ReadyQueue));					
					currentProcess = (Process *) deQueue(ReadyQueue);
					//printf("\tSize DEPOIS deQueue EM READY CPU. %ld\n", getSize(ReadyQueue));	
					printf("\tTime: %f", now);
					printf("\tPID: %d ", currentProcess->id);

					//get time wait in Ready Queue in this turn
					currentProcess->currentReadyQueueTime += getTimeElapsed(readyTime);

					printf("\tCurrent Burst: %f", getSeconds(currentProcess->currentCPUTime));
					printf("\t\tTotal CPU remaining: %f", getSeconds(currentProcess->totalCPUTimeRemaining));

					//move the process to the CPU
					enQueue(CPU, currentProcess);

					//moving the process to the CPU counts +1 context switch made by the simulator
					contextSwitchCount++;

					if(currentProcess->totalCPUTimeRemaining <= quantum)
					{
						Event *processFinished = (Event *) malloc (sizeof(Event));
						processFinished->type = PROCESS_FINISHED;
						processFinished->eventTime = now + getSeconds(currentProcess->totalCPUTimeRemaining);
						Priority_enQueue(EventQueue, processFinished, processFinished->eventTime);
					}
					else
					{
						if(quantum <= currentProcess->currentCPUTime)
						{
							Event *quantumExp = (Event *) malloc (sizeof(Event));
							quantumExp->type = QUANTUM_EXPIRATION;
							quantumExp->eventTime = now + getSeconds(quantum);
							Priority_enQueue(EventQueue, quantumExp, quantumExp->eventTime);
						}
						else
						{
							Event *IOinterrupt = (Event *) malloc (sizeof(Event));
							IOinterrupt->type = IO_INTERRUPT;
							IOinterrupt->eventTime = getSeconds(now * 1000000 + currentProcess->currentCPUTime);
							Priority_enQueue(EventQueue, IOinterrupt, IOinterrupt->eventTime);
						}

					}

					break;
				}
				

				case QUANTUM_EXPIRATION:
				{
					printf("\n***** QUANTUM EXPIRATION  | ");
					printf("\tTime: %f", now);
					Process *interrupted;

					//preemptingg the process from CPU
					interrupted = deQueue(CPU);

					//preempting the process from CPU results in 1 context switch
					contextSwitchCount++;

					printf("\tPID: %d ", interrupted->id);
					interrupted->totalCPUsofar += quantum;
					interrupted->currentCPUTime -= quantum;
					interrupted->totalCPUTimeRemaining -= interrupted->currentCPUTime;

					if(interrupted->currentCPUTime <= 0)
					{
						if(interrupted->bound == CPU_BOUND)
							interrupted->currentCPUTime = randomizeUniformly(10000, 20000);
						else
							interrupted->currentCPUTime = randomizeUniformly(1000, 2000);
					}	

					printf("\tTotal CPU so far: %.7f", getSeconds(interrupted->totalCPUsofar));
					printf("\tTotal CPU remaining: %.7f", getSeconds(interrupted->totalCPUTimeRemaining));

					enQueue(ReadyQueue, interrupted);
					
					Event *ready_cpu = (Event *) malloc (sizeof(Event));
					ready_cpu->type = READY_CPU;
					ready_cpu->eventTime = now;
					Priority_enQueue(EventQueue, ready_cpu, ready_cpu->eventTime);

					break;
				}

				case IO_INTERRUPT:
				{
					printf("\n***** I/O INTERRUPT ***** | ");
					printf("\tTime: %f", now);
					Process *returning;
					returning = (Process *) malloc (sizeof(Process));
					returning = deQueue(CPU);
					printf("\tPID: %d ", returning->id);
					returning->interrupts++;

					//This part of the code is put under comments because it didn't work correctly, causing segmentation fault on the Ready Queue
					//Also, commenting this part reduces the number of READY-CPU events in the simulation, and the consequence is the low number of processes that finish at the end of simulation
					/*if(!EmptyQueue(ReadyQueue))
					{
						Event *ready_cpu = (Event *) malloc (sizeof(Event));
						ready_cpu->type = READY_CPU;
						ready_cpu->eventTime = now; //+ getTimeElapsed(beginning);
						Priority_enQueue(EventQueue, ready_cpu, ready_cpu->eventTime);
					}*/

					returning->totalCPUsofar += returning->currentCPUTime;
					returning->totalCPUTimeRemaining -= returning->currentCPUTime;

					if(returning->bound == CPU_BOUND)
					{
						returning->currentCPUTime = randomizeUniformly(10000, 20000);
						avgCPUTime_forIO += returning->currentCPUTime;
					}
					else
					{
						returning->currentCPUTime = randomizeUniformly(1000, 2000);
						avgCPUTime_forCPU += returning->currentCPUTime;
					}
						

					enQueue(IOQueue, returning);

					Event *IOfinished = (Event *) malloc (sizeof(Event));
					IOfinished->type = FINISHED_IO;
					timeIO = randomizeExponentialy(avgIOserviceTime);

					returning->currentIOTime += timeIO;
					IOfinished->eventTime = now + getTimeElapsed(beginning) + getSeconds(timeIO);
					printf("\tService time: %.6f", getSeconds(timeIO));
					
					Priority_enQueue(EventQueue, IOfinished, IOfinished->eventTime);

					//printf("\tSize of Ready Queue ANTES de lançar READY QUEUE em IO INTERRUPT. %ld\n", getSize(ReadyQueue));

					break;
				}

				case FINISHED_IO:
				{
					printf("\n***** FINISHED IO ******* | ");
					printf("\tTime: %f", now);
					Process *backfromIO;
					backfromIO = deQueue(IOQueue);
					printf("\tPID: %d ", backfromIO->id);

					enQueue(ReadyQueue, backfromIO);
					if(EmptyQueue(CPU))
					{
						Event *ready_cpu = (Event *) malloc (sizeof(Event));
						ready_cpu->type = READY_CPU;
						ready_cpu->eventTime = now + getTimeElapsed(beginning);
						Priority_enQueue(EventQueue, ready_cpu, ready_cpu->eventTime);
					}

					break;
				}

				case PROCESS_FINISHED:
				{
					printf("\n***** PROCESS FINISHED ***** | ");
					printf("\tTime: %f", now);
					Process *f = deQueue(CPU);
					printf("\tPID: %d ", f->id);
					if(!EmptyQueue(ReadyQueue))
					{
						Event *ready_cpu = (Event *) malloc (sizeof(Event));
						ready_cpu->type = READY_CPU;
						ready_cpu->eventTime = now + getTimeElapsed(beginning);
						Priority_enQueue(EventQueue, ready_cpu, ready_cpu->eventTime);
					}

					turnaroundAll += f->currentIOTime + (f->currentReadyQueueTime * 1000000) + f->currentIOTime;


					processes_finished++;
					avgCPUutilization_forFinished += f->totalCPUsofar;
					avgReadyTimeAll += f->currentReadyQueueTime;
					
					if(f->bound == IO_BOUND)
					{
						totalIOProcessCompleted++;
						avgCPUtime_forFinished_CPU += f->totalCPUsofar;		
						avgReadyTime_forIO += f->currentReadyQueueTime;
						avgIOinterrupts_forIO += f->interrupts;		
						turnaroundIO += f->currentIOTime + (f->currentReadyQueueTime * 1000000) + f->currentIOTime;
					}
					else
					{
						totalCPUProcessCompleted++;
						avgCPUtime_forFinished_IO += f->totalCPUsofar;
						avgReadyTime_forCPU += f->currentReadyQueueTime;
						avgIOinterrupts_forCPU += f->interrupts;
						turnaroundCPU += f->currentIOTime + (f->currentReadyQueueTime * 1000000) + f->currentIOTime;
					}

					free(f);
					break;
				}

			}

			free(r);
		}//if(!EmptyPriorityQueue(EventQueue))
		else
			break;
	}//while

	releaseQueue(ReadyQueue);
	releaseQueue(CPU);
	releasePriorityQueue(EventQueue);

	avgCPUtime /= num_processes;

	printf("\n");

	printf("\nOVERALL\n");
	printf("\tSimulation time: %d\n", total_simulation);
	printf("\tCreated %d processes.\n", num_processes);
	printf("\tAverage CPU time: %f\n", getSeconds(avgCPUtime));
	printf("\tCPU utilization: %.1f of %d seconds of simulation (%.2f%%)\n", getSeconds(totalCPUutilization), total_simulation, getSeconds((100 * totalCPUutilization)/total_simulation));
	printf("\tTotal time in context switches: %f\n", getSeconds(contextSwitchCount * context_switch));

	//The average of finished process is very low compared to Richard Kline version
	printf("\nNumber of processes that finished: %ld (See the notes in code to see why is so low)\n", processes_finished); 
	//I believe that is because I commented the READY-CPU event in the I/O interrupt case (line 285)

	printf("\tTotal IO Processes completed: %.2f%%\n", (double) (totalIOProcessCompleted * 100)/processes_finished);
	printf("\tTotal CPU Processes completed: %.2f%%\n", (double) (totalCPUProcessCompleted * 100)/processes_finished);
	printf("\tAverage ready waiting time: %.2f\n", avgReadyTimeAll / processes_finished);
	printf("\tAverage turnaround: %.2f\n", getSeconds(turnaroundAll / processes_finished));

	printf("\nNumber of I/O bound processes that finished: %ld\n", totalIOProcessCompleted);
	printf("\tAverage CPU time for finished I/O bound processes: %.2f seconds.\n", getSeconds(avgCPUtime_forFinished_IO / totalIOProcessCompleted));
	printf("\tAverage ready waiting time for I/O bound processes: %.2f seconds.\n", avgReadyTime_forIO / totalIOProcessCompleted);
	printf("\tAverage turnaround for I/O processes: %.2f\n", getSeconds(turnaroundIO / totalIOProcessCompleted));
	printf("\tAverage I/O interrupts: %ld\n", (int) avgIOinterrupts_forIO / totalIOProcessCompleted);

	printf("\nNumber of CPU bound processes that finished: %ld\n", totalCPUProcessCompleted);
	printf("\tAverage CPU time for finished CPU bound processes: %.2f seconds.\n", getSeconds(avgCPUtime_forFinished_CPU / totalCPUProcessCompleted));
	printf("\tAverage ready waiting time for CPU bound processes: %.2f seconds.\n", avgReadyTime_forIO / totalIOProcessCompleted);
	printf("\tAverage turnaround for CPU processes: %.2f seconds.\n", getSeconds(turnaroundCPU / totalCPUProcessCompleted));
	printf("\tAverage I/O interrupts: %ld\n", (int) avgIOinterrupts_forIO / totalCPUProcessCompleted);

	printf("\nEND OF SIMULATION.\n");

	return 0;

} //main

/****************** IMPLEMENTATION OF FUNTIONS *******************/

void getParameters(int *total_seconds, double *quantum, int *context_switch, int *avgProcessLength, int *avgCreationTime, 
	int *IOBoundPct, int *avgIOserviceTime)
{
	FILE *input;
	char path_file[100] = "files/params10.txt";
	char buffer[MAX_LINE_LENGTH];

	buffer[0] = 0;

	input = fopen(path_file, "r");

	if(input != NULL)
	{
		if(!(fgets(buffer, MAX_LINE_LENGTH, input) == NULL))
		{
			if(!feof(input))
				fscanf(input, "%d%*[^\n] %lf%*[^\n] %d%*[^\n] %d%*[^\n] %d%*[^\n] %d%*[^\n] %d%*[^\n]\n", total_seconds, quantum, context_switch, avgProcessLength, avgCreationTime, IOBoundPct, avgIOserviceTime);
			else
				printf("End of file reached.\n");
			
		}
		else
		{
			printf("Error reading first line to buffer\n");	
		}

		fclose(input);
	}
	else
	{
		printf("Input is NULL!!!\n");
	}
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

double randomizeUniformly(int high, int low)
{
	int aux;

	if(low > high) //swap the values if needed
	{
		aux = low;
		low = high;
		high = aux;
	}

	//getting the current time for the seed in microseconds increases the randomness of rand 
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);

	double x;

	int range = high - low + 1;
	//x = low + (int) ((double) rand() / (RAND_MAX + 1) * range);
	x = ((rand() % (range)) + low);
	x = (double) x;

	if(low > 1000) //The program will only truncate for high value ranges
	{
		x *= 10000;
		x = (long int) x;
		x /= 10000;
	}

	return x;
}

double randomizeExponentialy(int parameter)
{
	double x;
	
	//getting the current time for the seed in microseconds increases the randomness of rand 
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);

	x = -parameter * log ((float)rand() / RAND_MAX );

	x *= 10000;
	x = (long int) x;
	x /= 10000; 

	return x;
}

double getSeconds(double us)
{
	return us / 1000000;
}

Process *newProcess()
{
	int index;
	
	Process *p = (Process *) malloc (sizeof(Process));
	p->id = num_processes;
	num_processes++;
	p->time_created = getTimeElapsed(beginning);
	p->totalCPUTimeRemaining = randomizeExponentialy(avgProcessLength);
	p->currentReadyQueueTime = 0.000000;
	p->currentIOTime = 0.000000;
	p->totalCPUsofar = 0.000000;
	p->interrupts = 0;

	index = randomizeUniformly(99,0);
	p->bound = probability[index];
	
	if(p->bound == CPU_BOUND)
		p->currentCPUTime = randomizeUniformly(10000, 20000);
	else
		p->currentCPUTime = randomizeUniformly(2000, 4000);

	return p;
}