// File:	my_pthread.c
// Author:	Yujie REN
// Date:	09/23/2017

// name:
// username of iLab:
// iLab Server:

#include "my_pthread_t.h"

// Checks if library is properly initialized
char initialized = 0;
// Exit context
ucontext_t exitContext;
// Checks if sheduler should be blocked
char block = 0;
// Pointer to the currently running thread's tcb
tcb * currentTcb = NULL;
// High priority queue
struct tcbQueue hpq = { NULL, NULL };

// Initializes a tcb, the context further
// be defined
tcb * initializeTcb() {
	tcb * ret = malloc(sizeof(tcb));
	ret->done = 0;
	ret->retVal = NULL;
	ret->waiter = NULL;
	return ret;
}
// Create the tcb Queue
tcbQueue * createQueue(tcb * thread) {
	struct threadQueueNode * node = malloc(sizeof(struct threadQueueNode));
	node->thread = thread;
	node->previous = NULL;
	node->next = NULL;
	struct threadQueue queue = malloc (sizeof(struct threadQueue));
	queue->front = node;
	queue->rear = node;
	queue->size = queue->size + 1;
}
	
	
	
// Enqueue <thread> onto the queue
void enqueueTcb(tcb * thread, struct tcbQueue * queue) {

	struct tcbQueueNode * node = malloc(sizeof(struct tcbQueueNode));
	node->thread = thread;
	node->previous = NULL;

	if (queue->tcbQueueTail == NULL) {
		node->next = NULL;
		queue->tcbQueueHead = node;
		queue->tcbQueueTail = node;

	} else {
		node->next = queue->tcbQueueHead;
		queue->tcbQueueHead->previous = node;
		queue->tcbQueueHead = node;
	}
}
void enqueueTcb(tcb * thread, tcbQueue * queue) {

	struct threadQueueNode * node = malloc(sizeof(struct threadQueueNode));
	node->thread = thread;
	node->previous = NULL;

	if (queue->size == 0) {
		node->next = NULL;
		queue->front = node;
		queue->rear = node;
		queue->size = queue->size + 1;

	} else {
		node->next = queue->rear;
		queue->rear->previous = node;
		queue->rear = node;
		queue->size = queue->size + 1;
	}
}

// Dequeues a tcb, returns NULL if
// no tcb
tcb * dequeueTcb(struct tcbQueue * queue) {

	if (queue->tcbQueueTail == NULL) {
		return NULL;

	} else {
		tcb * ret = queue->tcbQueueTail->thread;
		struct tcbQueueNode * newTail = queue->tcbQueueTail->previous;
		free(queue->tcbQueueTail);
		queue->tcbQueueTail = newTail;
		return ret;
	}
}

tcb * dequeueTcb(tcbQueue * queue) {

	if (queue-> size == 0) {
		return NULL;

	}
	else if(queue-> size == 1){
		struct threadQueueNode * node = queue->front;
		node->next = NULL;
		node->previous = NULL;
		queue->front = NULL;
		queue->rear = NULL;
		queue->size = 0;
		return node->thread;
	}
	else if(queue-> size == 2){
		struct threadQueueNode * node = queue->front;
		node->previous->next = NULL;
		queue->front = node->previous;
		node->next = NULL;
		node->previous = NULL;
		queue->size = 1;
		return threadQueueHead->thread;
	}
	else{
		struct threadQueueNode * node = queue->front;
		node->previous->next = NULL;
		queue->front = node->previous;
		node->next = NULL;
		node->previous = NULL;
		queue->size = queue->size - 1;
		return node->thread;
	}
}

void schedule(int signum) {

	// Only run if scheduler isn't blocked
	if (!block) {

		// Block the scheduler
		block = 1;

		// Get next tcb in the queue
		tcb * nextTcb = dequeueTcb(&hpq);

		// Switch context if there was a thread in
		// the queue. Unblock the scheduler.
		if (nextTcb != NULL) {
			tcb * previousTcb = currentTcb;
			currentTcb = nextTcb;
			
			// If <previousTcb> is NULL then setcontext to the
			// <newTcb>, else swap it with <previousTcb>
			if (previousTcb == NULL) { 
				block = 0;
				setcontext(&(nextTcb->context));

			} else {
				enqueueTcb(previousTcb, &hpq);
				block = 0;
				swapcontext(&(previousTcb->context), &(nextTcb->context));
			}
	
		} else {
			block = 0;
		}
	}
}

/* create a new thread */
int my_pthread_create(my_pthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg) {

	block = 1;

	// This block only runs on the first call to
	// my_pthread_create, setting the itimer and
	// exitContext
	if (!initialized) {

		initialized = 1;

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
		timer->it_value.tv_usec = CONTEXT_SWITCH_TIME;
		timer->it_interval.tv_sec = 0;
		timer->it_interval.tv_usec = CONTEXT_SWITCH_TIME;
		setitimer(ITIMER_VIRTUAL, timer, NULL);

		// Cretae tcb for first caller
		currentTcb = initializeTcb();
	}

	block = 0;

	// Create the new thread
	tcb * newTcb = initializeTcb();
	getcontext(&(newTcb->context));
	void * newThreadStack = malloc(TEMP_SIZE);
	newTcb->context.uc_link = &exitContext;
	newTcb->context.uc_stack.ss_size = TEMP_SIZE;
	newTcb->context.uc_stack.ss_sp = newThreadStack;
	makecontext(&(newTcb->context), function, 1, arg);
	*thread = newTcb;
	enqueueTcb(newTcb, &hpq);

	return 0;
};

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield() {
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
		enqueueTcb(currentTcb->waiter, &hpq);
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
		currentTcb = dequeueTcb(&hpq);
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
	return 0;
};

/* aquire the mutex lock */
int my_pthread_mutex_lock(my_pthread_mutex_t *mutex) {
	return 0;
};

/* release the mutex lock */
int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex) {
	return 0;
};

/* destroy the mutex */
int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex) {
	return 0;
};
