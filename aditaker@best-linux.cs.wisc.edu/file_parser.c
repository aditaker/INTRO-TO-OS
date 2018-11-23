#include "file_parser.h"

const int TARGET = 0;
const int COMMAND = 1;
const int BLANK = 2;
const int INVALID = 3;
const int COMMENT = 4;
const int BUFFERSIZE = 1024;

//Returns a new line read from the file. Sets type variable to line type.
char *getLine(FILE *fp, int *type, int *moreLines, int lineNo) {
    int c;
    int strLength;
    char *str = NULL, *tmp = NULL;
    str = (char *)malloc(BUFFERSIZE);
    if(str == NULL) {
        printf("Malloc failed.");
        exit(1);
    }
    strLength = 0;
    tmp = str;
    while((c = getc(fp)) != EOF && c != 10 && c != 0) {
        *tmp = c;
        tmp++;
        strLength++;
        if(strLength >= BUFFERSIZE) {	//Effective buffer size is reduced by 1 to add '\0' at the end of the string.
            fprintf(stderr, "Line size of line no. %d larger than buffer.\n", lineNo);
            exit(1);
        }
    }
    *tmp = '\0';
    if(c == 0)	//Line contains null character.
    {
    	fprintf(stderr, "Line no. %d contains null character.\n", lineNo);
        exit(1);
    }
    
    if(c == EOF)    //All lines in the file are read.
    {
    	*moreLines = 0;
    	if(!strLength)
    	{
    		*type = BLANK;
    		return str;	
    	}
	}    
    
    if(c == 10 && !strLength)
    	*type = BLANK;
    else if(str[0] == '\t')  //Line contains a command.
        *type = COMMAND;
    else if(str[0] == '#')  //Line contains a comment.
        *type = COMMENT;
    else if(str[0] == ' ')
    	*type = INVALID;
    else {
        tmp = (char *)malloc(BUFFERSIZE);   //Copy string before strtok.
        if(tmp == NULL) {
            printf("Malloc failed.");
            exit(1);
        }
        strcpy(tmp, str);
        char *token = strtok(tmp, ":");   //Get the first token delimited by ':'.
		if(token == NULL)
		{
			*type = INVALID;
		}
		else 
		{
			*type = TARGET;
		}
        free(tmp);
    }
    return str;
}

//Parse the makefile to create a list of targets.
targetList *parseFile(char *fileName) {

    //Create and initialize empty list for targets read from file.
    targetList *list = (targetList *)malloc(sizeof(targetList));
    if(list == NULL) {
        printf("Malloc failed.");
        exit(1);
    }
    initTargetList(list);
    
    FILE *fp;

    if(fileName)    //Try to open specified makefile.
    {
        fp = fopen(fileName, "r");
	    if(fp == NULL)
        {
            fprintf(stderr, "Error in opening file %s\n", fileName);
            exit(1);
        }
    }
    else    //Try to open 'makefile' or 'Makefile'.
    {
        fp = fopen("makefile", "r");
        if(fp == NULL)
        {
            fp = fopen("Makefile", "r");
            if(fp == NULL)
            {
                fprintf(stderr, "No makefile found\n");
                exit(1);
            }
        }
    }
    
    char *line;
    int type;
    int lineNo = 1;
    int moreLines = 1;
    target *t = NULL;
    command *c = NULL;
    while(moreLines) {
        line = getLine(fp, &type, &moreLines, lineNo);  //Read a line from the file.
        if(type == TARGET)  //Add the target to the target list.
        {
            t = createTarget(line);     //Create a struct for target.
            addTargetToList(list, t);   //Add the struct to thelist.
            free(line);
        }
        else if(type == COMMAND) {  //Add the command to the target.
            if(t == NULL) {
                printf("No target for command: %s\n", line + 1);
                free(line);
            }
            else 
            {
                c = (command *)malloc(sizeof(command));
                if(c == NULL) {
                    printf("Malloc failed.");
                    exit(1);
                }
                c->cmd = line + 1;      //Ignore the first tab in the command.
                c->next = NULL;
                addCommandToTarget(t, c);
            }
        }
        else if(type == INVALID) {
            fprintf(stderr, "%d Invalid: %s\n", lineNo, line);
            exit(1);
        }
        else if(type == COMMENT || type == BLANK)
            free(line);     //Drop comment/blank line.
        lineNo++;
    }
    return list;
}
