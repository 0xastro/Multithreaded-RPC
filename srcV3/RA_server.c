/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "RA.h"
#include <stdatomic.h>
typedef struct QueueLock {
        pthread_mutex_t		lock;		
        pthread_cond_t		cond;
       	unsigned int		worker;
        unsigned int		waiter;
} Qlock;

static Qlock qqlock;

/*----------------------------------------------*/
/* 
 *Ticket Based Queue Prototyping
 *Anti-Starvation FIFO Queue lock/unlock 
 */
/*----------------------------------------------*/

int Queue_init(Qlock *qlock);
int Queue_Lock(Qlock *qlock);
int Queue_UnLock(Qlock *qlock);
/*----------------------------------------------*/
/*
 *Global Shared resources handeled by the threads
 */
/*----------------------------------------------*/
pthread_mutex_t 	lock;				/*Lock for the Queue*/
static atomic_uint 	rsrc_pvt=100;			/*Number of Private Resources*/
unsigned int 		init=0;				/*Flag to initialize the @lock once*/
unsigned int 		Req_Rsrc,Rep_Rsrc;		/*Requested Resources, Reply*/
unsigned int 		QFlag;

/*----------------------------------------------*/
/* 
 *  Function: @allocate_2_svc is Called
 *  by the thread @serv_request 	        
 */
/*----------------------------------------------*/
bool_t
allocate_2_svc(rsrc_req *argp, reply *result, struct svc_req *rqstp)
{
	bool_t			retval;
	unsigned int		work;
	unsigned int 		req_rsrc;
	/*Initialize the lock*/
	if (!init) {
    		pthread_mutex_init(&lock, NULL);
		Queue_init(&qqlock);
	}
	
	/* Save the Requested Resources
	 * lock only if the amount of requested resources
	 * are not satisfied
	 */
	req_rsrc=argp->req;

	pthread_mutex_lock(&lock);
	while (req_rsrc > rsrc_pvt) {
		printf("DEBUG:\t\targp->req %d\n\n",argp->req);
		/*
		 * If the number of requested resources is satisfied
	 	* so QFlag:=1
	 	* else threads will wait until there's a broadcast when 
	 	* resources are released
	 	*/
		//pthread_mutex_lock(&lock); 
		QFlag=Queue_Lock(&qqlock); 

	}
	rsrc_pvt= __sync_sub_and_fetch( (unsigned int*) & (rsrc_pvt),  req_rsrc);
	pthread_mutex_unlock(&lock);


	/* ------------
	 * Do some work
	 * -----------
	 */
  
	
	printf("[START:\t] Thread id = %ld, arg = %d\n",pthread_self(),argp->req);
	printf("[UPDATE:\t] rsrc_pvt = %u \n",rsrc_pvt);
	result->rep = 2*(argp->req);
	work=rand()%2;
	sleep(work); 
	

	return retval;
}


/*----------------------------------------------*/
/* Function: @release_2_svc is Called
 *  by the thread @serv_request 	        	*/
/*----------------------------------------------*/
bool_t
release_2_svc(rsrc_req *argp, reply *result, struct svc_req *rqstp)
{
	bool_t retval;
	unsigned int 		req_rsrc_release;
	/* 
	 * >critical section
	 * [DeAllocation]: Update the resources number */

	req_rsrc_release=argp->req;	

	if (req_rsrc_release) { 		

		/*Release the resources*/
		rsrc_pvt= __sync_add_and_fetch( (unsigned int*) & (rsrc_pvt),  req_rsrc_release);
		/*BroadCast to indicate that the resources have been released*/
		Queue_UnLock(&qqlock);
		printf("[UPDATE:\t] rsrc_pvt = %u \n",rsrc_pvt);
	  	printf("[END  :\t] Thread id = %ld is done\n",pthread_self());
	}

	return retval;
}

int
resourceallocator_2_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}



int Queue_init(Qlock *qlock){

	if(!init) { 

    		pthread_mutex_init(&(qlock->lock), NULL);
		pthread_cond_init(&(qlock->cond), NULL);

		qlock->worker = 1U;	//set the LSB to 1
		qlock->waiter = 0U;	
	}
	init=1;
	return 1;
}

/*If req_rsrc are OK, UNLOCK and procceed and after finishing broadcast
 cuz may be there's some threads are waiting
 	blocked threads scenario:
	thread1 request not eniugh num of rsrc
*/
int Queue_Lock(Qlock *qlock) { 

	atomic_uint	myTicket;
	myTicket = __sync_add_and_fetch( (int*) & (qlock->waiter), (int) 1)%254;
	printf("myTicket:%d\n", myTicket);
	
	pthread_mutex_lock(&qlock->lock);


	/*Block If It's not my turn*/
	while (myTicket != qlock->worker) {
		printf("Ticket %d, worker %d \n\n",myTicket,qlock->worker);
		pthread_cond_wait(&qlock->cond,&qlock->lock);
	}
	
	pthread_mutex_unlock(&qlock->lock);
	return 1;
}

int Queue_UnLock(Qlock *qlock) { 
	/*Increase the waiters counter*/
	qlock->worker = __sync_add_and_fetch( (int*) & (qlock->worker), (int) 1)%254;
	pthread_mutex_lock(&qlock->lock);
	pthread_cond_broadcast(&qlock->cond);	
	pthread_mutex_unlock(&qlock->lock);	
	return 1;
}
