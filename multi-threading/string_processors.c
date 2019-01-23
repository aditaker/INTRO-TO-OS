#include "queue.h"
#include "string_processors.h"

const int BUFFERSIZE = 1024;

//Reads a string from stdin and enqueues it to a queue
void Reader(void *arg)
{
    Queue *q = (Queue *)arg;
    int eof = 0;
    int c;
    int str_length;
    char *str = NULL, *tmp = NULL;
    while(!eof) {
	    str = (char *) malloc (BUFFERSIZE);
	    str_length = 0;
	    tmp = str;
	    while((c = getchar()) != EOF && c != 10) {
	    	*tmp = c;
	    	tmp++;
	    	str_length++;
	    	if(str_length >= BUFFERSIZE) {
	    		while((c = getchar()) != EOF && c != 10);
	    		free(str);
    			fprintf(stderr, "Line size greater than buffer. Dropping it\n");
	    		break;
	    	}
		}
    	if(str_length < BUFFERSIZE && c != EOF) {
    		*tmp = '\0';
    		EnqueueString(q, str);
    	}
		if(c == EOF)
			eof = 1;
    }
    EnqueueString(q, NULL);
    printf("Reader exiting\n");
    pthread_exit(NULL);
}

//Dequeues a string from reader's queue and replaces spaces with *
//Then enqueues the string to another queue
void Munch1(void *arg) {
	Queue *qLeft = ((TwoQueues *)arg)->qLeft;
	Queue *qRight = ((TwoQueues *)arg)->qRight;
	char *str = NULL, *tmp = NULL;
	while(1) {
		str = DequeueString(qLeft);
		tmp = str;
		if(tmp != NULL) {
			while((tmp = index(tmp, ' ')) != NULL) {
				*tmp = '*';
			}
			EnqueueString(qRight, str);
		}
		else {
			EnqueueString(qRight, NULL);
			break;
		}
	}
	printf("Munch1 exiting\n");
	pthread_exit(NULL);
}

//Dequeues a string from a queue and converts lowercase to uppercase
//Then enqueues the string to another queue
void Munch2(void *arg) {
	Queue *qLeft = ((TwoQueues *)arg)->qLeft;
	Queue *qRight = ((TwoQueues *)arg)->qRight;
	char *str = NULL, *tmp = NULL;
	while(1) {
		str = DequeueString(qLeft);
		tmp = str;
		if(tmp) {
			while(*tmp != '\0') {
				if(islower(*tmp)) 
					*tmp = toupper(*tmp);
				tmp++;
			}
			EnqueueString(qRight, str);
		}
		else {
			EnqueueString(qRight, NULL);
			break;
		}
	}
	printf("Munch2 exiting\n");
	pthread_exit(NULL);
}

//Dequeues a string from a queue and writes it to standard output.
void Writer(void *arg) {
	Queue *q = (Queue *)arg;
	char *str = NULL, *tmp = NULL;
    while(1) {
		str = DequeueString(q);
		tmp = str;
		if(tmp) {
			printf("%s\n", str);
			free(str);
		}
		else {
			break;
		}
	    
    }
    printf("Writer exiting\n");
    pthread_exit(NULL);
}
