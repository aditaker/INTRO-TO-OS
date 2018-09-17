#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<errno.h>
#include<stdbool.h>
#include<string.h>

struct Node{
	int process_id;
	struct Node* next;
};

void is_valid_processID(char *path) {
	DIR* directory;
	if((directory = opendir(path)) == NULL) {
		perror("Process does not exist");
		exit(1);	
	}
}

void readStat(char *path, char *state, int *utime, int *stime ) {
	FILE *fptr = fopen(path, "r");
    if ( fptr == NULL ){
        perror("Erro opening file");
        exit(1);
    }
    else {
    	fscanf(fptr,"%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %u %u", state, utime, stime);
    	printf("%c\n",*state);
    	printf("%d\n",*utime);
    	printf("%d\n",*stime);
    }
}

void readStatm(char *path, int *vmem) {
	FILE *fptr = fopen(path, "r");
    if ( fptr == NULL ){
        perror("Erro opening file");
        exit(1);
    }
    else {
    	fscanf(fptr,"%d", vmem);
    	printf("%d\n",*vmem);
    }
}

int main() {
	DIR *directory;
	FILE *fptr;
	struct dirent *process;
	bool pid = true;
	int p = 4947; 
	struct Node *processList;
	char *path = malloc(50);
	char state;
	int utime, stime, vmem;
	if(pid) {
		sprintf(path,"/proc/%d",p);
		is_valid_processID(path);
		processList = (struct Node*)malloc(sizeof(struct Node));
		processList->process_id = p;
		// char *statpath = malloc(50);
		// strcpy(statpath, path);
		// strcat(statpath,"/stat");
		// readStat(statpath, &state, &utime, &stime);
		// free(statpath);
		// char *statmpath = malloc(50);
		// strcpy(statmpath, path);
		// strcat(statmpath,"/statm");
		// readStatm(statmpath, &vmem);
		// free(statmpath);
	}
	else {
		if((directory = opendir("/proc")) == NULL) {
			perror("Error opening directory");
			exit(1);	
		}
		else {
			processList = (struct Node*)malloc(sizeof(struct Node));
			struct Node* head = processList;
			struct Node *prev;
			while((process = readdir(directory)) != NULL) { 
				if(process->d_name[0] >= '0' && process->d_name[0] <= '9') {
					processList->process_id = atoi(process->d_name);
					struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
					processList -> next = temp;
					prev = processList;
					processList = processList -> next;
				}
			}
			prev->next = NULL;
			while(head) {
				printf("%d\n",head->process_id);
				head = head->next;
			}
		}
	}			
	free(path);
	return 0;
}
