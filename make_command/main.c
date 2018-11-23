//Aditya Rungta
//Nachiket Kshatriya

#include <getopt.h>

#include "file_parser.h"
#include "build_spec_repr.h"
#include "build_spec_graph.h"

void print_usage() {
	printf("Usage: ./537make [-f <filename>] [target]\n");
    exit(1);
}

int main(int argc , char **argv) {
	
	//Parse commandline arguments.
	int opt;
	char *fileName = NULL;
	char *target = NULL;

	if(argc > 4)
		print_usage();
	
	while ((opt = getopt(argc, argv, "-f:")) != -1) {
		switch (opt) {
        case 'f':
           	if(optarg)
				fileName = optarg;
        	break;
		case 1:
			target = optarg;
			break;
        default:
            print_usage();
        }
	}

	//Parse the makefile to create a list of targets.
    targetList *tList = parseFile(fileName);
    
	//Create a graph from the list of targets.
    nodeList *nList = createGraph(tList);

	//Check if the graph contains cyclic dependency.
    chekcCyclicDependency(nList);

	/** Build the specified target( or first target in makefile in no target is
	 *  provided)
	 */
    if(target)
		buildGraph(nList, target);
	else
		buildGraph(nList, tList->head->name);
    return 0;
}
