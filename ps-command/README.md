Aditya Rungta
Nachiket Kshatriya

The program is divided into three modules:
    1. option_processing: This module contains functions for processing cli arguments and set flags accordingly.
    2. proc_files_parser: Deals with the parsing and printing of required process info from stat, statm, status and 					cmdline files.
    3. process_list: Maintains a list of processes for which info is to be
                        shown.

Program working:
    1. The program starts with parsing the user cli.
    2. If user provides a pid, then only that pid is added to the list. Else
        /proc directory is traversed to find all processes belonging to the user
        and added to the list.
    3. Processes are removed from the list one by one and info is printed for   
        each process according to the flags set by the user.

How to run the project:
	1. Run make command inside the project's directory.
	2. An executable with the name of 537ps will be created.
	3. Run the command ./537ps [-p <pid>] [-s] [-U] [-S] [-v] [-c] to get the desired process information.
        
    
