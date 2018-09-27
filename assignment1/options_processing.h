//Aditya Rungta
//Nachiket Kshatriya

#ifndef OPTIONS_PROCESSING_H
#define OPTIONS_PROCESSING_H

#define BUFFER_SIZE 1024

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//Structure to store cli flags.
typedef struct inputFLags{
    int pid, sFlag, UFlag, SFlag, vFlag, cFlag;
} inputFlags;

//Initialize the flags to default values.
void init_flags(inputFlags *flags);

//Checks if the string is a valid number.
int is_number(char *str);

//Get the flags from cli.
void get_input_flags(int argc, char *argv[], inputFlags *flags);

#endif
