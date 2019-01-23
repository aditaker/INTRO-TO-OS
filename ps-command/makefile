CC = gcc
WARNING_FLAGS = -Wall -Wextra -pedantic
EXE = 537ps

x: y t

y: 
	echo 'HELLO'

t: y

all: main.o options_processing.o process_list.o proc_files_parser.o
	$(CC) -o $(EXE) main.o options_processing.o process_list.o proc_files_parser.o
	rm *.o

main.o: main.c options_processing.h process_list.h proc_files_parser.h
	$(CC) $(WARNING_FLAGS) -c main.c

options_procssing.o: options_processing.c options_processing.h
	$(CC) $(WARNING_FLAGS) -c options_processing.c

process_list.o: process_list.c process_list.h
	$(CC) $(WARNING_FLAGS) -c process_list.c

proc_files_parser.o: proc_files_parser.c proc_files_parser.h
	$(CC) $(WARNING_FLAGS) -c proc_files_parser.c

clean:
	rm $(EXE) *.o