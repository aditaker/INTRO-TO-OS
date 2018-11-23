#include "queue.h"


void init(queue *q) {
	q->arr = (char **) malloc (sizeof(char *) * QSIZE);
	for(int i = 0; i < QSIZE; i++) {
		q->arr[i] = NULL;
	}
    q->f = 0;
    q->r = 0;
    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->enqueueBlockCount = 0;
    q->dequeueBlockCount = 0;
    sem_init(&q->full, 0, 0);
    sem_init(&q->empty, 0, QSIZE);
}

void enqueue(queue *q, char *c) {
	if(sem_trywait(&q->empty) != 0) {
		q->enqueueBlockCount++;
		sem_wait(&q->empty);
	}
    q->enqueueCount++;
	q->arr[q->r] = c;
	q->r = (q->r + 1) % QSIZE;
	sem_post(&q->full);
}

char *dequeue(queue * q) {
	if(sem_trywait(&q->full) != 0) {
		q->dequeueBlockCount++;
		sem_wait(&q->full);
	}
	char *c = q->arr[q->f];
	q->dequeueCount++;
	q->f = (q->f + 1) % QSIZE;
	sem_post(&q->empty);
	return c;
}