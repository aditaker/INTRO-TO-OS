CC=gcc
WARNING_FLAGS=-Wall -Wextra
EXE=537make

# the -g flag at all gcc compilation stages ensures that you can use gdb to debug your code
$(EXE): main.o build_spec_graph.o file_parser.o build_spec_repr.o execute_command.o
	$(CC) -g -o $(EXE) main.o build_spec_graph.o file_parser.o build_spec_repr.o execute_command.o

main.o: main.c build_spec_graph.h file_parser.h build_spec_repr.h
	$(CC) -g $(WARNING_FLAGS) -c main.c

build_spec_graph.o: build_spec_graph.c build_spec_graph.h
	$(CC) -g $(WARNING_FLAGS) -c build_spec_graph.c

file_parser.o: file_parser.c file_parser.h
	$(CC) -g $(WARNING_FLAGS) -c file_parser.c

build_spec_repr.o: build_spec_repr.c build_spec_repr.h
	$(CC) -g $(WARNING_FLAGS) -c build_spec_repr.c

execute_command.o: execute_command.c execute_command.h
	$(CC) -g $(WARNING_FLAGS) -c execute_command.c

clean:
	rm -f $(EXE) *.o

recompile: clean $(EXE)
