Aditya Rungta
Nachiket Kshatriya

The program is divided into two modules:
1.) queue : This module contains the queue implementation with functions to enqueue and dequeue strings.
2.) string_processing : This function contains functions to read from stdin, process strings in different ways and write stdout.

Working of the program:
1.) It spawns 4 threads(Reader, Munch1, Munch2, Writer), each of which is responsible for different tasks.
2.) Reader thread is responsible for reading line by line from stdin and pushing each line to a queue.
3.) Munch1 thread reads a line from the queue to which Reader enqueued and processes it and enqueues it to another queue.
4.) Munch2 thread reads a line from the queue to which Munch1 enqueued and processes it and enqueues it to another queue.
5.) Writer thread reads a line from the queue to which Munch2 enqueued and writes it to standard output.
6.) When stdin reacheds EOF, Reader thread terminates, followed by all the other threads.

How to run the program :
1.) Run make command inside project's directory. An executable named prodcomm will be generated.
2.) Run ./prodcomm < input.txt
3.) Desired output will be displayed.