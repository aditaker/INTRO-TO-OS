#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define BUFFERSIZE 16

typedef struct queue {
    char **arr;
    int f, r;
	int enqueueCount, dequeueCount, enqueueBlockCount, dequeueBlockCount;
    sem_t full, empty;
} queue;

typedef struct queues {
	queue *ql, *qr;
} queues;

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

void reader(void *arg)
{
    queue *q = (queue *)arg;
    int eof = 0;
    int c;
    int str_length;
    char *str = NULL, *tmp = NULL;
    while(!eof) {
	    str = (char *) malloc (BUFFERSIZE + 1);
	    str_length = 0;
	    tmp = str;
	    while((c = getchar()) != EOF && c != 10) {
	    	*tmp = c;
	    	tmp++;
	    	str_length++;
	    	if(str_length > BUFFERSIZE) {
	    		while((c = getchar()) != EOF && c != 10);
	    		free(str);
    			printf("Line size greater than buffer. Dropping it\n");
	    		break;
	    	}
		}
    	if(str_length <= BUFFERSIZE && c != EOF) {
    		*tmp = '\n';
    		enqueue(q, str);
    	}
		if(c == EOF)
			eof = 1;
    }
    enqueue(q, NULL);
    printf("Reader exiting\n");
    fprintf(stderr, "Reader: enqueueCount = %d enqueueBlockCount = %d\n", q->enqueueCount, q->enqueueBlockCount);   
}

void munch1(void *arg) {
	queue *ql = ((queues *)arg)->ql;
	queue *qr = ((queues *)arg)->qr;
	char *str = NULL, *tmp = NULL;
	while(1) {
		str = dequeue(ql);
		
		tmp = str;
		if(tmp != NULL) {
			while(*tmp != '\n') {
				if(*tmp == ' ') 
					*tmp = '*';
				tmp++;
			}
			enqueue(qr, str);
		}
		else {
			enqueue(qr, NULL);
			break;
		}
	}
	printf("Munch1 exiting\n");
    fprintf(stderr, "Munch1: enqueueCount = %d dequeueCount = %d enqueueBlockCount = %d dequeueBlockCount = %d\n", qr->enqueueCount, ql->dequeueCount, qr->enqueueBlockCount, ql->dequeueBlockCount);
}

void munch2(void *arg) {
	queue *ql = ((queues *)arg)->ql;
	queue *qr = ((queues *)arg)->qr;
	char *str = NULL, *tmp = NULL;
	while(1) {
		str = dequeue(ql);
		tmp = str;
		if(tmp) {
			while(*tmp != '\n') {
				if(*tmp >= 97 && *tmp <= 122) 
					*tmp -= 32;
				tmp++;
			}
			enqueue(qr, str);
		}
		else {
			enqueue(qr, NULL);
			break;
		}
	}
	printf("Munch2 exiting\n");
    fprintf(stderr, "Munch2: enqueueCount = %d dequeueCount = %d enqueueBlockCount = %d dequeueBlockCount = %d\n", qr->enqueueCount, ql->dequeueCount, qr->enqueueBlockCount, ql->dequeueBlockCount);
}

void writer(void *arg) {
	queue *q = (queue *)arg;
	char *str = NULL, *tmp = NULL;
    while(1) {
		str = dequeue(q);
		tmp = str;
		if(tmp) {
			while(*tmp != '\n') {
				putchar(*tmp);
				tmp++;
			}
			putchar('\n');
			free(str);
		}
		else {
			break;
		}
	    
    }
    printf("Writer exiting\n");
    fprintf(stderr, "Writer: dequeueCount = %d dequeueBlockCount = %d\n", q->dequeueCount, q->dequeueBlockCount);
}

int main()
{
	pthread_t t1, t2, t3, t4;
    queue q1, q2, q3;
    queues q12, q23;
    init(&q1);
    init(&q2);
    init(&q3);
    q12.ql = &q1;
    q12.qr = &q2;
    q23.ql = &q2;
    q23.qr = &q3;
	pthread_create(&t1, NULL, (void *) &reader, (void *)&q1);
	pthread_create(&t2, NULL, (void *) &munch1, (void *)&q12);
	pthread_create(&t3, NULL, (void *) &munch2, (void *)&q23);
	pthread_create(&t4, NULL, (void *) &writer, (void *)&q3);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	return 0;
}
