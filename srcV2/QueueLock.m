#include <pthread.h>

#define TICKET_LOCK_INITIALIZER { PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER }

typedef struct ticket_lock
{
	pthread_mutex_t lock;
	pthread_cond_t cond;
	unsigned int queue_head,
				 queue_tail;
} ticket_lock_t;

void ticket_lock(ticket_lock_t *ticket)
{
    unsigned long queue_me;

    pthread_mutex_lock(&ticket->lock);
    queue_me = ticket->queue_tail++;
    while (queue_me != ticket->queue_head)
    {
        pthread_cond_wait(&ticket->cond, &ticket->lock);
    }
    pthread_mutex_unlock(&ticket->lock);
}

void ticket_unlock(ticket_lock_t *ticket)
{
    pthread_mutex_lock(&ticket->lock);
    ticket->queue_head++;
    pthread_cond_broadcast(&ticket->cond);
    pthread_mutex_unlock(&ticket->lock);
}
