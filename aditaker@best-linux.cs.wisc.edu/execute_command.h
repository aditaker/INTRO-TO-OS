//Aditya Rungta
//Nachiket Kshatriya

#ifndef EXECUTE_COMMAND_H
#define EXECUTE_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//Execute the given command using fork-exec.
void executeCommand(char *);

#endif