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

// Enqueue <thread> onto the queue
void enqueueThread(tcb * thread) {

	struct threadQueueNode * node = malloc(sizeof(struct threadQueueNode));
	node->thread = thread;
	node->previous = NULL;

	if (threadQueueTail == NULL) {
		node->next = NULL;
		threadQueueHead = thread;
		threadQueueTail = thread;

	} else {
		node->next = threadQueueHead;
		threadQueueHead->previous = node;
		threadQueueHead = node;
	}
}

// Dequeues a thread
tcb * dequeueThread() {

	if (threadQueueTail == NULL) {
		return NULL;

	} else {
		tcb * ret = threadQueueTail->thread;
		threadQueueTail = threadQueueTail->previous;
		return ret;
	}
}

/* create a new thread */
int my_pthread_create(my_pthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg) {

	if (!initialized) {

		void * exitStack = malloc(TEMPSIZE);

		exitContext.uc_stack.ss_size = TEMPSIZE;
		exitContext.uc_stack.ss_sp = exitStack;

		getcontext(&exitContext);
		makecontext(&exitContext, my_pthread_exit, 0);

		initialized = 1;
	}

	ucontext_t new;

	void * threadStack = malloc(TEMPSIZE);

	new.uc_link = &exitContext;
	new.uc_stack.ss_size = TEMPSIZE;
	new.uc_stack.ss_sp = threadStack;

	getcontext(&new);
	makecontext(&new, function, 1, arg);
	
	return 0;
};

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield() {
	return 0;
};

/* terminate a thread */
void my_pthread_exit(void *value_ptr) {
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
