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
// Head of the thread queue
static struct threadQueueNode * threadQueueHead = NULL;
// Tail of the thread queue
static struct threadQueueNode * threadQueueTail = NULL;
// Checks if sheduler should be blocked
char block = 0;
// Pointer to the currently running thread's tcb
tcb * currentTcb = NULL;

// Returns the tcb with <tid>, returns NULL if it doesn't
// exist
tcb * getTcb(my_pthread_t tid) {
	tcb * ret = tid;
	if (ret->tid == tid) { return ret; }
	else { return NULL; }
}

// Enqueue <thread> onto the queue
void enqueueTcb(tcb * thread) {

	struct threadQueueNode * node = malloc(sizeof(struct threadQueueNode));
	node->thread = thread;
	node->previous = NULL;

	if (threadQueueTail == NULL) {
		node->next = NULL;
		threadQueueHead = node;
		threadQueueTail = node;

	} else {
		node->next = threadQueueHead;
		threadQueueHead->previous = node;
		threadQueueHead = node;
	}
}

// Dequeues a tcb, returns NULL if
// no tcb
tcb * dequeueTcb() {

	if (threadQueueTail == NULL) {
		return NULL;

	} else {
		tcb * ret = threadQueueTail->thread;
		struct threadQueueNode * newTail = threadQueueTail->previous;
		free(threadQueueTail);
		threadQueueTail = newTail;
		return ret;
	}
}

void schedule(int signum) {

	// Only run if scheduler isn't blocked
	if (!block) {

		// Block the scheduler
		block = 1;

		// Get next tcb in the queue
		tcb * nextTcb = dequeueTcb();

		// Switch context if there was a thread in
		// the queue. Unblock the scheduler.
		if (nextTcb != NULL) {
			tcb * previousTcb = currentTcb;
			currentTcb = nextTcb;
			
			// If <previousTcb> is NULL then setcontext
			// else swapcontext
			if (previousTcb == NULL) { 
				tcb * curr = malloc(sizeof(tcb));
				enqueueTcb(curr);
				block = 0;
				swapcontext(&(curr->context), &(nextTcb->context));
				curr->context.uc_link = &exitContext;

			} else {
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

	// Block the scheduler
	block = 1;

	// This block only runs on the first call to
	// my_pthread_create
	if (!initialized) {

		// Make this block run once
		initialized = 1;

		// Catch itimer signal
		signal(SIGVTALRM, schedule);

		// Save pthread_exit context
		void * exitStack = malloc(TEMP_SIZE);
		exitContext.uc_stack.ss_size = TEMP_SIZE;
		exitContext.uc_stack.ss_sp = exitStack;
		getcontext(&exitContext);
		makecontext(&exitContext, my_pthread_exit, 0);

		// Start itimer
		struct itimerval * timer = malloc(sizeof(struct itimerval));
		timer->it_value.tv_sec = 0;
		timer->it_value.tv_usec = CONTEXT_SWITCH_TIME;
		timer->it_interval.tv_sec = 0;
		timer->it_interval.tv_usec = CONTEXT_SWITCH_TIME;
		setitimer(ITIMER_VIRTUAL, timer, NULL);
	}

	// Create tcb for new thread
	tcb * newTcb = malloc(sizeof(tcb));
	getcontext(&(newTcb->context));
	void * newThreadStack = malloc(TEMP_SIZE);
	newTcb->context.uc_link = &exitContext;
	newTcb->context.uc_stack.ss_size = TEMP_SIZE;
	newTcb->context.uc_stack.ss_sp = newThreadStack;
	makecontext(&(newTcb->context), function, 1, arg);
	newTcb->tid = &newTcb; // Tid is set to the address of <newTcb>
	enqueueTcb(newTcb); // Save the new tcb

	// Unblock the scheduler
	block = 0;
	
	return 0;
};

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield() {
	return 0;
};

/* terminate a thread */
void my_pthread_exit(void *value_ptr) {
	block = 1;
	currentTcb = NULL;
	block = 0;
	schedule(0);
};

/* wait for thread termination */
int my_pthread_join(my_pthread_t thread, void **value_ptr) {
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
