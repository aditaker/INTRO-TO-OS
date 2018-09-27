//Aditya Rungta
//Nachiket Kshatriya

#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "proc_files_parser.h"

typedef struct process{
	int pid;
	struct process* next;
} process;

typedef struct processList{
	process *head, *tail;
} processList;

//Get next process pid from the list.
int dequeue_process(processList *list);

//Add a process to the list.
void enqueue_process(processList *list, int pid);

//Add all user process to the list.
void get_all_user_processes(processList *list);

//Initialize process list.
void init_process_list(processList *list);

#endif