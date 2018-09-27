//Aditya Rungta
//Nachiket Kshatriya

#include "proc_files_parser.h"

//Get the real UID for a process.
//Real UID is the first integer on the 9th line of process status file.
int get_process_userID(int pid){
	char *line = NULL;
	char *path = malloc(BUFFER_SIZE);
	int count = 0;
	int read;
	int uid = 0;
	size_t len = 0;

	sprintf(path, "/proc/%d/status", pid);
	FILE *fptr = fopen(path, "r");

	if(fptr == NULL){
        perror("Error opening status file");
		free(path);
        return false;
    }
    else{
    	while((read = getline(&line, &len, fptr)) != -1){
	        count++;
	        if(count == 8)
	        	break;
    	}
    	fscanf(fptr, "%*s %d", &uid);

		free(line);
		free(path);
		
		return uid;
    }
}

//Read the state, utime and stime values for a process from stat file.
void get_stat_values(int pid, char *state, unsigned int *utime, unsigned int *stime ){
	char *path = malloc(BUFFER_SIZE);
	sprintf(path, "/proc/%d/stat", pid);
	FILE *fptr = fopen(path, "r");

    if(fptr == NULL){
        perror("Erro opening stat file");
    }
    else{
    	fscanf(fptr,"%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %u %u", state, utime, stime);
		fclose(fptr);
		free(path);
    }
}

//Read the memory used by a process from statm file.
int get_statm_value(int pid){
	int vmem;
	char *path = malloc(BUFFER_SIZE);
	sprintf(path, "/proc/%d/stat", pid);
	FILE *fptr = fopen(path, "r");

    if(fptr == NULL){
        perror("Erro opening statm file");
		return -1;
    }
    else{
    	fscanf(fptr,"%d", &vmem);
		fclose(fptr);
		free(path);
    	return vmem;
    }
}

//Check if the process belongs to the user.
int is_valid_user_process(int pid, int userID){
	if(get_process_userID(pid) == userID)
		return true;
	return false;
}

//Check if the process is valid or not.
//A valid process will contain a directory entry in /proc directory.
int is_valid_process(int pid){
	char *path = malloc(BUFFER_SIZE);
	sprintf(path, "/proc/%d", pid);
	DIR *directory = opendir(path);

	if(directory == NULL){
        return false;
    }

	closedir(directory);
	free(path);
	return true;
}

//Function to print string containing multiple \0 characters.
void special_print(char *str, int size) {
	int i = 0;
	while(i < size){
		printf("%s", str + i);
		i = i + strlen(str + i) + 1;
		if(i < size)
			printf(" ");
	};
}

//Print the command by which the process started.
//The command is contained in cmdline file of process directory.
void print_cmdline(int pid){
	char *buffer = malloc(BUFFER_SIZE + 1);
	buffer[BUFFER_SIZE] = '\0';
	char *path = malloc(BUFFER_SIZE);
	int read_count = 0;
	sprintf(path, "/proc/%d/cmdline", pid);
	FILE *fptr = fopen(path, "r");

	if(fptr == NULL){
        perror("Erro opening cmdline file");
    }
    else{
		printf("[ ");
		while((read_count = fread(buffer, 1, BUFFER_SIZE, fptr)) != 0)
    		special_print(buffer, read_count);
		printf(" ]");
		fclose(fptr);
		free(path);
    }
}

//Print the process information according to the various flags set.
void print_process_info(int pid, inputFlags flags){
	int vmem;
	unsigned int utime, stime;
	char state;
	printf("%d: ", pid);
	if(is_valid_process(pid)){
		if(flags.sFlag || flags.UFlag || flags.SFlag)
			get_stat_values(pid, &state, &utime, &stime);
		if(flags.sFlag)
			printf("state=%c ", state);
		if(flags.UFlag)
			printf("utime=%u ", utime);
		if(flags.SFlag)
			printf("stime=%u ", stime);
		if(flags.vFlag){
			vmem = get_statm_value(pid);
			printf("memory=%d ", vmem);
		}
		if(flags.cFlag)
			print_cmdline(pid);
		printf("\n");
	}
}
