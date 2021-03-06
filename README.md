# Round-Robin-Scheduling-Simulation

Author: Yasmin B. A. da Silva

This is a project for my Operating System's class that I took in the fall of 2014 at Pace University, New York. 
The project consistis in simulating a Process scheduling alghorithm used by some Operating Systems: the Round Robin algorithm. 

This simulator was programmed using C language. 

In Round Robin (which is a preemptive scheduling algorithm), each process stays on the CPU for its execution by one and no more than 1 interval called quantum, which has an arbitrary value. When the quantum time expires, the current running process is preempted from the CPU and goes back to the end of the Ready Queue, and the next process waiting in the Ready Queue is picked up right away. 

The simulation uses a Regular Queue to manage the processes, and a Priority Queue to manage the events according to the event's creation time. Note that even the CPU will be treated as a queue with size = 1. Both Regular and Priority Queues are implemented using Linked Lists and one can see the code in the following files: 

Header files: LinkedList.h, Queue.h and PriorityQueue.h

Source Code: LinkedList.c, Queue.c and PriorityQueue.c

The main_program.c file consists on a while loop that executes while current simulation time is less than total simulation time.
A first new Process event is created outside the loop to start the simulation.
When the loop begins, one event will be dequeued from the Event's Queue, the time 'now' is updated with the event's time, and the event will be processed according to the following 6 situations based on its type:

NEW PROCESS: A new process will be created, and all random information that is needed will be provided for that process. Right after that, the process will be placed at the Ready Queue. If the CPU is empty, this event will trigger an new READY-CPU event to place that new process on the CPU.

READY-CPU: This event means that the scheduler will move the first process in the Ready Queue to the CPU, that at the moment is empty. 
- If the process is CPU-bound, its current CPU burst is longer than the quantum length so it will trigger a new QUANTUM EXPIRATION event at the time now + quantum; 
- If it is I/O bound, its current CPU burst is shorter than the quantum so it will trigger a new I/O INTERRUPT event at the time now + current CPU burst;

QUANTUM EXPIRATION: When a process is running and the quantum time expires, this process will be removed from CPU and place back at the end of Ready Queue. That will trigger another READY-CPU event because now the CPU is empty so the next process in the line can take the CPU and execute its instructions.

I/O INTERRUPT: This event means that a process need to leave the CPU to perform some I/O operations. The process now is placed on an I/O queue. A new FINISHED I/O event is triggered at the time now + process's I/O service time.

FINISHED I/O: When a process finishes its I/O operations, it will be placed at the end of Ready Queue to wait until it gets on the CPU again to resume its execution.

PROCESS FINISHED: When a process finishes, it is removed from the CPU or the Ready Queue and terminated.

Each time that an event is triggered, it is placed on the Event Queue until it will be dequeued in the next loop's execution and thus continue the simulation.

The program also provides a set of data to show at the end an overall performance of the simulation.

INPUTS (params10.txt): 

  - Total Simulation time (seconds);
  - Quantum's size (microseconds);
  - Context Switch time (microseconds);  
  - Average Process Length (microseconds);
  - Average Creation time (microseconds);
  - Percentage of I/O bound processes (0-100);
  - average I/O service time (microseconds);
  
OUTPUTS (output10.txt and myOutput.txt):

Standard:
  - Event type;
  - Time that the event was created;
  - Process'ID that triggered the event;
  
For the process creation event:
  - Bound type: if the process is CPU Bound (10000-20000 microseconds long) or I/O bound (2000-4000 microseconds long);
  - Total CPU burst;
  - Current CPU burst;
  - Time when the next process will be created;
  
For the event that put the process on CPU:
  - Current CPU burst;
  - CPU time remaining;
  
For I/O interruption event:
  - Service time for I/O operations made by that process;
  
For Quantum Expiration event:
  - Total time of CPU utilized by that process so far; 
  - Total CPU time remaining;
