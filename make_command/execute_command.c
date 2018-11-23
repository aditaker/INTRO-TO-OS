//Aditya Rungta
//Nachiket Kshatriya

#include "execute_command.h"
#include "file_parser.h"

//Execute the given command using fork-exec.
void executeCommand(char *command) {
    printf("%s\n", command);
    fflush(stdout);
    pid_t pid = fork();
	if(pid == 0) {
		char *str = (char *)malloc(BUFFERSIZE);
		if(str == NULL) {
			printf("Malloc failed.");
			exit(1);
		}
        strcpy(str, command);
		char **argv = (char **)malloc(sizeof(char *) * BUFFERSIZE);
		if(argv == NULL) {
			printf("Malloc failed.");
			exit(1);
		}
		char *token = strtok(str, " ");
		if(token == NULL) {			//Command does not contain any arguments.
			execlp(command, command, NULL);	//Execute the command.
        	exit(1);
		}
		int j = 0;
		int fd;
		while(token != NULL) {
			if(strcmp(token, ">") == 0)
			{
				token = strtok(NULL, " ");
				if(token == NULL)
				{
					fprintf(stderr, "Invalid command: %s\n", command);
					exit(1);
				}
				close(1);		//Redirect stdout to file.
				fd = open(token, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
				if(fd == -1)
				{
					fprintf(stderr, "Cannot open file: %s\n", token);
					exit(1);
				}
			}
			else if(strcmp(token, "<") == 0)
			{
				token = strtok(NULL, " ");
				if(token == NULL)
				{
					fprintf(stderr, "Invalid command: %s\n", command);
					exit(1);
				}
				close(0);		//Redirect stdin to file.
				fd = open(token, O_RDONLY);					
				if(fd == -1)
				{
					fprintf(stderr, "Cannot open file: %s\n", token);
					exit(1);
				}
			}
			else
			{
				argv[j++] = token;	//Create an array of arguments.
			}
			token = strtok(NULL, " ");
		}
		argv[j] = NULL;
		execvp(argv[0], argv);	//Execute the command.
        exit(1);
	}
	else {
        int status;
		wait(&status);		//Wait for child to exit.
        if(status != 0) {
            printf("Failed to execute command: %s\n", command);
            exit(1);
        }
	}
}
