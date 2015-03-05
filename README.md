# Round-Robin-Scheduling-Simulation

Author: Yasmin B. A. da Silva

This is a project for my Operating System's class that I took in the fall of 2014 at Pace University, New York. 
The project consistis in simulating a Process scheduling alghorithm used by some Operating Systems: the Round Robin algorithm. 

This simulator was programmed using C language. 

In Round Robin (which is a preemptive scheduling algorithm), each process stays on the CPU for its execution by one and no more than 1 quantum, which have an arbitrary value. When the quantum time expires, the current running process is preempted from the CPU and goes back to the end of the Ready Queue, and the next process waiting in the Ready Queue is picked up right away. 

Inputs (params10.txt): 

  - Total Simulation time (seconds);
  - Quantum's size (microseconds);
  - Context Switch time (microseconds);  
  - Average Process Length (microseconds);
  - Average Creation time (microseconds);
  - Percentage of I/O bound processes (0-100);
  - average I/O service time (microseconds);
  
Outputs (output10.txt and myOutput.txt):
  - Event type;
  - Time that the event was created;
  - Process'ID that triggered the event;
  
  For the process creation event:
  - Bound type: if the process is CPU Bound (10000-20000 microseconds long) or I/O bound (2000-4000 microseconds long);
  - Total CPU burst;
  - Current CPU burst;
  - Time when the next process will be created
  
  For the event that put the process on CPU:
  - Current CPU burst;
  - CPU time remaining
  
  For I/O interruption event:
  - Service time for I/O operations made by that process;
  
  For Quantum Expiration event:
  - Total time of CPU utilized by that process so far; 
  - Total CPU time remaining;








