//Aditya Rungta
//Nachiket Kshatriya

#include "options_processing.h"
#include "process_list.h"
#include "proc_files_parser.h"

int main(int argc, char *argv[]){
	
    int pid;

	//Initialize linked list to stored process ids.
	processList pList;
	init_process_list(&pList);
    
	//Initialize structure to store cli flags.
    inputFlags flags;
    init_flags(&flags);
    get_input_flags(argc, argv, &flags);
    
	if(flags.pid == -1)
		//No pid is provided by user. => Print all user processes.
		get_all_user_processes(&pList);
    else {
		if(is_valid_process(flags.pid))
			//Valid pid is provided by user.
			enqueue_process(&pList, flags.pid);
		else{
			printf("Process %d not found.\n", flags.pid);
			exit(1);
		}
	}
	
	//Removed pids from the list and print info for each.
	while((pid = dequeue_process(&pList)) != 0)
		print_process_info(pid, flags);

	return 0;
}
