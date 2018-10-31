all: main.o options_processing.o process_list.o proc_files_parser.o
	echo "Hello World 1"

main.o: main.c options_processing.h process_list.h proc_files_parser.h
	echo "Hello World 2"

options_processing.o: options_processing.c options_processing.h process_list.o
	echo "Hello World 3"

process_list.o: process_list.c process_list.h options_processing.o
	echo "Hello World 4"

proc_files_parser.o: proc_files_parser.c proc_files_parser.h
	echo "Hello World 5"

clean:
	echo "Hello World"