CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = prodcomm

all: main.o queue.o string_processors.o
	$(CC) -o $(EXE) main.o queue.o string_processors.o -lpthread
	rm *.o

main.o: main.c queue.h string_processors.h 
	$(CC) $(WARNING_FLAGS) -c main.c

queue.o: queue.c queue.h
	$(CC) $(WARNING_FLAGS) -c queue.c
	
string_processors.o: string_processors.c string_processors.h
	$(CC) $(WARNING_FLAGS) -c string_processors.c

clean:
	rm $(EXE) *.o
