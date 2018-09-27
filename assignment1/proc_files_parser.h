//Aditya Rungta
//Nachiket Kshatriya

#ifndef PROC_FILES_PARSER_H
#define PROC_FILES_PARSER_H

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "options_processing.h"

//strcuture to store values read from stat file
typedef struct statValues{
	int state, utime, stime;
} statValues;

//Get real UID of a process.
int get_process_userID(int pid);

//Get the state, utime and stime values for a process.
void get_stat_values(int pid, char *state, unsigned int *utime, unsigned int *stime);

//Get the memory use by a process.
int get_statm_value(int pid);

//Check if the process is valid.
int is_valid_process(int pid);

//Check if the process is owned by the user.
int is_valid_user_process(int pid, int UID);

//Print the command by which the process started.
void print_cmdline(int pid);

//Print the process info according to the flags set.
void print_process_info(int pid, inputFlags flags);

#endif