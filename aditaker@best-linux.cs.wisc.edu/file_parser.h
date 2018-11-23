//Aditya Rungta
//Nachiket Kshatriya

#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "build_spec_repr.h"

//Types of line read from the file.
extern const int TARGET, COMMAND, BLANK, INVALID, COMMENT;
extern const int BUFFERSIZE;

//Pare the makefile to create a list of targets.
targetList *parseFile(char *);

#endif