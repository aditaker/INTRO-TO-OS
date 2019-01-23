#include "queue.h"

const int QSIZE = 10;


//Creates a queue which stores strings
Queue *CreateStringQueue(int size) {
	Queue *q = (Queue *) malloc (sizeof(Queue));
	q->arr = (char **) malloc (sizeof(char *) * size);
	for(int i = 0; i < size; i++) {
		q->arr[i] = NULL;
	}
    q->front = 0;
    q->rear = 0;
    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->enqueueBlockCount = 0;
    q->dequeueBlockCount = 0;
    sem_init(&q->full, 0, 0);
    sem_init(&q->empty, 0, size);
    return q;
}

//Enqueues a string to the queue
void EnqueueString(Queue *q, char *string) {
	if(sem_trywait(&q->empty) != 0) {
		q->enqueueBlockCount++;
		sem_wait(&q->empty);
	}
    q->enqueueCount++;
	q->arr[q->rear] = string;
	q->rear = (q->rear + 1) % QSIZE;
	sem_post(&q->full);
}


//Dequeues a string from the queue
char *DequeueString(Queue *q) {
	if(sem_trywait(&q->full) != 0) {
		q->dequeueBlockCount++;
		sem_wait(&q->full);
	}
	char *string = q->arr[q->front];
	q->dequeueCount++;
	q->front = (q->front + 1) % QSIZE;
	sem_post(&q->empty);
	return string;
}

//Prints all the queue stats
void PrintQueueStats(Queue *q) {
	fprintf(stderr, "EnqueueCount: %d\n", q->enqueueCount);
	fprintf(stderr, "DequeueCount: %d\n", q->dequeueCount);
	fprintf(stderr, "EnqueueBlockCount: %d\n", q->enqueueBlockCount);
	fprintf(stderr, "DequeueBlockCount: %d\n", q->dequeueBlockCount);
}
