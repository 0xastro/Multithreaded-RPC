
#define TH_ID   pthread_self()

struct QueueLock {
        pthread_mutex_t		mutex;
        pthread_cond_t		cond;
       	unsigned int		worker;
        unsignet int		waiter;
};

unsigned int	init_lock;

/*
 * 1.Lock the mutex, Aquire the lock. 
 * if (the Number of requested resources > Number of Available Resources) 
 *      Block on the conditional variable: cond_wait
 *      increase the queue_waiters++
 * else
 *      Do the work ...
 *      if (Finished)
 *          Broadcast(); //wake up all the threads on the queue
 *
 * When woken:
 * Try to Aquire the mutex
 * If (counter != saved counter value)
 *     Block on the conditional variable: cond_wait     
 */

/* 
 * Queue_init: Initialize the qlock structures
 * only once by the first call
 */
int Queue_init(){
	
	if (!init) {
		pthread_mutex_init(&(qlock->lock), NULL);
		pthread_cond_init(&(qlock->->cond), NULL);
		qlock->->worker = 1U;	/*set the LSB to 1*/
		qlock->->waiter = 0U;	
	}
}

/* 
 * Queue_Lock
 *
 */
int Queue_Lock() { 
	/*Increase the waiter counter*/
	qlock->waiter = (qlock->waiter)+1;
	/*Save it to be compared later*/
	myTicket= qlock->waiter;

	pthread_mutex_lock(&qlock->lock);
	/*Block If It's not my turn*/
	while (myTicket != qlock->worker) {
		pthread_cond_wait(&qlock->cond,&qlock->lcok);
	}	
}
/* 
 * Queue_UnLock
 *
 */	
int Queue_UnLock() { 
	
	/*Increase the waiters counter*/
	qlock->worker = (qlock->worker)+1;
	pthread_cond_broadcast(&qlock->cond);	
	pthread_mutex_unlock(&qlock->lock);
		
}
