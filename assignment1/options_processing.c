//Aditya Rungta
//Nachiket Kshatriya

#include "options_processing.h"

//Print correct program usage info.
void print_usage() {
	printf("Incorrect usage\n");
	printf("Correct Usage: ./537ps [-p <pid>] [-s] [-U] [-S] [-v] [-c]\n");
    exit(1);
}

//Check if the flag needs to be enabled/disabled.
int checkFlag(char *optarg){
	if(optarg) {
		if(strlen(optarg) == 1 && optarg[0] == '-')
			return 0;
		else
			print_usage();
	}
	return 1;
}

//Set default flags.
void init_flags(inputFlags *flags){
    flags->pid = -1;
    flags->sFlag = 0;
    flags->UFlag = 1;
    flags->SFlag = 0;
    flags->vFlag = 0;
    flags->cFlag = 1;
}

//Check if the input process id is a valid number.
int is_number(char *str) {
	while(*str != '\0') {
		if(!isdigit(*str))
			return 0;
		str++;
	}
	return 1;
}

//Parse the cli flags.
void get_input_flags(int argc, char *argv[], inputFlags *flags){

	int opt;
    
    while ((opt = getopt(argc, argv, "-p:s::U::S::v::c::")) != -1) {
        switch (opt) {
        case 'p':
           	if(optarg && is_number(optarg))
            	flags->pid = atoi(optarg);
        	else
    			print_usage();
            break;
        case 's':
        	flags->sFlag = checkFlag(optarg);
            break;
        case 'U':
			flags->UFlag = checkFlag(optarg);
			break;
        case 'S':
			flags->SFlag = checkFlag(optarg);
			break;
        case 'v':
			flags->vFlag = checkFlag(optarg);
			break;
        case 'c':
			flags->cFlag = checkFlag(optarg);
			break;
        default:
            print_usage();
        }
    }
}
