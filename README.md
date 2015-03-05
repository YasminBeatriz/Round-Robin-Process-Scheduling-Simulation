# Round-Robin-Scheduling-Simulation

Author: Yasmin B. A. da Silva

This is a project for my Operating System's class that I took in the fall of 2014 at Pace University, New York. 
The project consistis in simulating a Process scheduling alghorithm used by some Operating Systems: the Round Robin algorithm. 

This simulator was programmed using C language. 

In Round Robin (which is a preemptive scheduling algorithm), each process stays on the CPU for its execution by one and no more than 1 quantum, which have an arbitrary value. When the quantum time expires, the current running process is preempted from the CPU and goes back to the end of the Ready Queue, and the next process waiting in the Ready Queue is picked up right away.

Inputs (text file): 

  - Total Simulation time (seconds);
  - Quantum's size (microseconds);
  - Context Switch time (microseconds);  
  - Average Process Length (microseconds);
  - Average Creation time (microseconds);
  - Percentage of I/O bound processes (0-100);
  - average I/O service time (microseconds);
  
Outputs:
  - Event type;
  - Time that the event was created;
  - Process'ID which triggered the event;
  - Bound type: if the process is CPU Bound (10000-20000 microseconds long) or I/O bound (2000-4000 microseconds long)





