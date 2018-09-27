//Aditya Rungta
//Nachiket Kshatriya

#include "process_list.h"

//Get next process from the list.
int dequeue_process(processList *pList){
	int pid = 0;
	process *tmp;
	if(pList->head != NULL){
		tmp = pList->head;
		pList->head = tmp->next;
		pid = tmp->pid;
		free(tmp);
	}
	return pid;
}

//Add a process to the list.
void enqueue_process(processList *pList, int pid){
	process *new = (process*)malloc(sizeof(process));
	new->pid = pid;
	new->next = NULL;

	if(pList->tail == NULL){
		pList->head = new;
		pList->tail = new;
	}
	else{
		pList->tail->next = new;
		pList->tail = new;
	}
}

//Add all user process to the list.
void get_all_user_processes(processList *pList){
	char *name;
	DIR *directory;
	int pid, userID;
	struct dirent *dirEntry;

	if((directory = opendir("/proc")) == NULL)
		perror("Error opening /proc directory");
	else {
		userID = getuid();
		while((dirEntry = readdir(directory)) != NULL){
			name = dirEntry->d_name;
			if(is_number(name)){
				pid = atoi(name);
				if(is_valid_user_process(pid, userID))
					enqueue_process(pList, pid);
			}
		}
		closedir(directory);
	}
}

//Initialize the process list.
void init_process_list(processList *pList){
	pList->head = NULL;
	pList->tail = NULL;
}
