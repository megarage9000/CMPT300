# Assignment 3
*Submission: John Ordoyo*

## Design choices:

### Semaphores:

Semaphore implementation was done in a separate file rather than in the same commands.c file. This is to isolate functionality from other functions such as send/receive/reply and overall ease of programming / mental capacity when implementing semaphores. The commands.c file would end up just fetching the results of calls in semaphores.c (ie. P(), V()).

### Process PCB's and Messages

Process PCB's contains PID, priority, state, and a Process Message. The Process Message consists of a sending pid, receiving pid, the actual message, and the message state which determines whether it is a send / receive. There is also a process tracker which is a statically allocated array that stores the associated queue with the given pid (which is used as an index). This allows for:

1. Fast process searching since processes in this system move from queue to queue so often

2. Ease of recycling pids and termination

### Kill and Exit

Both Kill and Exit in this case function similarly, as Exit simply calls Kill on the current process. 

### INIT PROCESS

The INIT PROCESS pid should be defined as an integer outside of the range of -1 <-> Number of nodes to avoid confusion between action result (FAILURE / SUCCESS) and process pids.

### Send / Receive / Reply

Send would always check if there happens to be a process waiting for the given send in the receive queue before blocking the calling the process. Receive would always check if there is a message send for the calling process before blocking. Reply would always check if there is a process blocked on send.

### Forking

Forking in this case would copy the process priority and message. State always change and PIDs are unique to every process.