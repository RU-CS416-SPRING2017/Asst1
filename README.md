Asst1: Adventures in Scheduling


API


Implementation


Scheduler

The following three macros defined in my_pthread.c determine the scheduling behavior:



NUM_PRIORITY_LVLS: An integer that defines the number of priority queue levels to create.

INTERRUPT_TIME: A number that defines the interval time in microseconds in which the scheduler should run. For this assignment, the value must be 25000 also written as 25mm.

BASE_TIME_SLICE: A number that defines the time slice of the threads in the highest priority queue.


The scheduler creates NUM_PRIORITY_LVLS queues, setting the time slice of the highest priority queue to BASE_TIME_SLICE microseconds. Each subsequent lower priority queue is set a time slice two times the time slice of the priority level immediately above it. Every INTERRUPT_TIME microseconds, the scheduler runs the next highest priority thread.

All new threads are initially given highest priority. When a running thread doesn't finish execution after it has completed its current  timeslice priority, its priority is decreased by one level before being placed back in the queue. In order to avoid starvation, a maintenance cycle is run, which will increase the priority of all the threads that have not run for a while.

When threads exit, the corresponding return value is saved in the thread's TCB. If a thread is waiting to join with a finished thread, the thread gets place back into the queue with the highest priority. If there is no thread waiting to join with the finished thread, the return value stays in memory for when another thread attempts to join.


Mutexes

When a mutex is initialized, a flag, a guard and a queue is allocated. When a thread tries to obtain(lock) a locked mutex, it is placed in the mutex's queue to woken up when the mutex is unlocked. When a thread unlocks a mutex, the mutex's queue is checked and if there are threads waiting, the lock is passed on to the next waiting thread. If there are no threads are waiting, the mutex simply becomes free(unlocked).


Extra Credit A

Every time a thread attempts to obtain the lock of a locked mutex, the priority of the thread is compared with the priority of the thread that currently has the lock. If the priority of the new thread is higher than the old one, the old thread inherits the priority of the new thread. Whenever a mutex is being unlocked and the lock is being passed on to a waiting thread, the waiting thread inherits the priority of the unlocking thread. Creating a sort of greedy algorithm, that has proven to be quite fast. 