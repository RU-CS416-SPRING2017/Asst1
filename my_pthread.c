// File:	my_pthread.c
// Author:	Yujie REN
// Date:	09/23/2017

// name:
// username of iLab:
// iLab Server:

#define TEMP_SIZE 4096
#define INTERRUPT_TIME 25 // In microseconds
#define BASE_TIME_SLICE INTERRUPT_TIME // In microseconds
#define NUM_PRIORITY_LVLS 4

#include "my_pthread_t.h"
#define UNLOCKED 0
#define LOCKED 1

// Checks if library is properly initialized
char initialized = 0;
// Exit context
ucontext_t exitContext;
// Checks if sheduler should be blocked
char block = 0;
// Pointer to the currently running thread's tcb
tcb * currentTcb = NULL;
// High priority queue
struct priorityQueue PQs[NUM_PRIORITY_LVLS];

// Returns the elapsed time
suseconds_t getElapsedTime(struct timeval * start, struct timeval * end) {
	time_t seconds = end->tv_sec - start->tv_sec;
	suseconds_t microseconds = end->tv_usec - start->tv_usec;
	if (seconds == 0) { return microseconds; }
	else { return (seconds * 1000000) + microseconds; }
}

// Initializes <priorityQueue>
void initializePQs() {
	int i;
	for (i = 0; i < NUM_PRIORITY_LVLS; i++) {
		if (i == 0) { PQs[i].timeSlice = BASE_TIME_SLICE; }
		else { PQs[i].timeSlice = PQs[i-1].timeSlice * 2; }
		PQs[i].queue.head = NULL;
		PQs[i].queue.tail = NULL;
	}
}


// Initializes a tcb, the context must further

// be defined
tcb * getTcb() {
	tcb * ret = malloc(sizeof(tcb));
	ret->done = 0;
	ret->retVal = NULL;
	ret->waiter = NULL;
	ret->priorityLevel = 0;
	gettimeofday(&(ret->start), NULL);
	ret->totalRunTime = 0;
	return ret;
}

// Enqueue <data> into <queue>
void enqueue(void * data, struct queue * queue) {

	struct queueNode * node = malloc(sizeof(struct queueNode));
	node->data = data;
	node->previous = NULL;

	if (queue->tail == NULL) {
		node->next = NULL;
		queue->head = node;
		queue->tail = node;

	} else {
		node->next = queue->head;
		queue->head->previous = node;
		queue->head = node;
	}
}

// Dequeues from <queue>
void * dequeue(struct queue * queue) {

	if (queue->tail == NULL) {
		return NULL;

	} else {
		void * data = queue->tail->data;
		struct queueNode * newTail = queue->tail->previous;
		if (newTail == NULL){
			queue->tcbQueueHead = NULL; // because you do not want to free if another pointer is still pointing to it. if newtail is null, then it is the only one therefore head must also change it's pointer to null
		}
		free(queue->tail);
		queue->tail = newTail;
		return data;
	}
}

// Returns the next tcb and removes it from the queue,
// NULL if no threads in queue
tcb * getNextTcb() {
	int i;
	for (i = 0; i < NUM_PRIORITY_LVLS; i++) {
		tcb * ret = dequeue(&(PQs[i].queue));
		if (ret != NULL) { return ret; }
	}
	return NULL;
}

void schedule(int signum) {

	// Only run if scheduler isn't blocked
	if (!block) {

		// Block the scheduler
		block = 1;

		// Get the runtime of the previous thread
		struct timeval now;
		gettimeofday(&now, NULL);
		suseconds_t previousTcbRunTime = 0;
		unsigned int previousTimeSlice = 0;
		if (currentTcb != NULL) {
			previousTcbRunTime = getElapsedTime(&(currentTcb->start), &now);
			previousTimeSlice = PQs[currentTcb->priorityLevel].timeSlice;
		}

		// If thread ran long enough, context switch
		if (previousTcbRunTime >= previousTimeSlice) {

			tcb * nextTcb = getNextTcb();

			// Switch context if there was a thread in
			// the queue. Unblock the scheduler.
			if (nextTcb != NULL) {

				tcb * previousTcb = currentTcb;
				currentTcb = nextTcb;

				// Set start time for the next thread
				gettimeofday(&(currentTcb->start), NULL);
				
				// If <previousTcb> is NULL then setcontext to the
				// <newTcb>, else swap it with <previousTcb>
				if (previousTcb == NULL) { 
					block = 0;
					setcontext(&(currentTcb->context));

				} else {

					// Decrease the priority level of <previousTcb> if not already
					// at the lowest priority
					if (previousTcb->priorityLevel < (NUM_PRIORITY_LVLS - 1)) {
						(previousTcb->priorityLevel)++;
					}

					enqueue(previousTcb, &(PQs[previousTcb->priorityLevel].queue));
					block = 0;
					swapcontext(&(previousTcb->context), &(currentTcb->context));
				}
		
			} else { block = 0; }

		}  else { block = 0; }
	}
}

/* create a new thread */
int my_pthread_create(my_pthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg) {

	block = 1;

	// This block only runs on the first call to
	// my_pthread_create, setting the itimer and
	// exitContext
	if (!initialized) {

		initializePQs();

		// Cretae tcb for first caller
		currentTcb = getTcb();

		// Catch itimer signal
		signal(SIGVTALRM, schedule);

		// Save pthread_exit context
		getcontext(&exitContext);
		void * exitStack = malloc(TEMP_SIZE);
		exitContext.uc_link = NULL;
		exitContext.uc_stack.ss_size = TEMP_SIZE;
		exitContext.uc_stack.ss_sp = exitStack;
		makecontext(&exitContext, my_pthread_exit, 1, NULL);

		// Start itimer
		struct itimerval * timer = malloc(sizeof(struct itimerval));
		timer->it_value.tv_sec = 0;
		timer->it_value.tv_usec = INTERRUPT_TIME;
		timer->it_interval.tv_sec = 0;
		timer->it_interval.tv_usec = INTERRUPT_TIME;
		setitimer(ITIMER_VIRTUAL, timer, NULL);

		initialized = 1;
	}

	block = 0;

	// Create the new thread
	tcb * newTcb = getTcb();
	getcontext(&(newTcb->context));
	void * newThreadStack = malloc(TEMP_SIZE);
	newTcb->context.uc_link = &exitContext;
	newTcb->context.uc_stack.ss_size = TEMP_SIZE;
	newTcb->context.uc_stack.ss_sp = newThreadStack;
	makecontext(&(newTcb->context), function, 1, arg);
	*thread = newTcb;
	enqueue(newTcb, &(PQs[0].queue));

	return 0;
};

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield() {

	block = 1;

	tcb * nextTcb = getNextTcb();
	
	if (nextTcb != NULL) {

		tcb * previousTcb = currentTcb;
		currentTcb = nextTcb;

		// Set start time for the next thread
		gettimeofday(&(currentTcb->start), NULL);

		enqueue(previousTcb, &(PQs[previousTcb->priorityLevel].queue));
		block = 0;
		swapcontext(&(previousTcb->context), &(currentTcb->context));
	}

	block = 0;
	return 0;
};

/* terminate a thread */
void my_pthread_exit(void *value_ptr) {

	block = 1;

	currentTcb->done = 1;
	currentTcb->retVal = value_ptr;

	// If the exiting thread has another thread
	// waiting on it, put the waiting thread in
	// the queue so it can be run later
	if (currentTcb->waiter != NULL) {
		currentTcb->waiter->priorityLevel = 0;
		enqueue(currentTcb->waiter, &(PQs[0].queue));
	}

	currentTcb = NULL;

	block = 0;
	schedule(0);
};

/* wait for thread termination */
int my_pthread_join(my_pthread_t thread, void **value_ptr) {

	block = 1;

	// Retrieve the tcb of the joining thread
	tcb * joining = thread;

	// If the the joining thread isn't done,
	// refrence the waiting (this) thread in
	// the joining thread's tcb and start
	// waiting
	if (!(joining->done)) {
		joining->waiter = currentTcb;
		currentTcb = getNextTcb();
		gettimeofday(&(currentTcb->start), NULL);
		block = 0;
		swapcontext(&(joining->waiter->context), &(currentTcb->context));
	}

	block = 0;

	// If <value_ptr> is not null, make it point to the
	// joining thread's return value.
	if (value_ptr != NULL) { *value_ptr = joining->retVal; }

	return 0;
};

/* initial the mutex lock */
int my_pthread_mutex_init(my_pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
	// UNLOCKED -> lock is available LOCKED -> lock is unavailable 
	mutex->lock = UNLOCKED;
	mutex->guard = UNLOCKED;
	mutex->
	return 0;
};

/* acquire the mutex lock */
int my_pthread_mutex_lock(my_pthread_mutex_t *mutex) {
	// need thread information
/*	while(1){
		while(mutex->lock == LOCKED);
		if(__sync_lock_test_and_set(&(mutex->lock), 1) == UNLOCKED) {break};
	}
	*/
	while(__sync_lock_test_and_set(&(mutex->guard), 1) == LOCKED);
	if (mutex->lock == UNLOCKED) {
		mutex->lock = LOCKED; // lock is acquired
		mutex->guard = UNLOCKED;
	} else {
		enqueue(mutex->waitQueue, &currentTcb); //need thread info
		mutex->guard = UNLOCKED;
		futex_wait(mutex,val); //waits until value changes //value has to equal value at address mutex
	}
	return 0;
};

/* release the mutex lock */
int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex) {
	while(__sync_lock_test_and_set(&(mutex->guard), 1) == LOCKED);
	if((mutex->waitQueue->)){
		mutex->lock = UNLOCKED;
	} else{
		futex_wake(dequeue(mutex->waitQueue)); //futex wake thread at this address
	}
	mutex->guard = UNLOCKED;
	return 0;
};

/* destroy the mutex */
int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex) {
	return 0;
};
