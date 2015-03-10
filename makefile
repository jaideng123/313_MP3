# makefile

all: memtest

ackerman.o: ackerman.c 
	gcc -c -g -lm ackerman.c

my_allocator.o : my_allocator.c
	gcc -c -g -lm my_allocator.c

memtest.o : memtest.c
	gcc -c -g -lm memtest.c

memtest: memtest.o ackerman.o my_allocator.o
	gcc -o -g -lm memtest.o ackerman.o my_allocator.o

