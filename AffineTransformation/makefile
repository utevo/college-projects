CC = gcc
CFLAGS = -g -Wall -m64 

all: main.o x86_function.o
	$(CC) $(CFLAGS) -o program main.o x86_function.o -lSDL -lm

x86_function.o: x86_function.s
	nasm -f elf64 -o x86_function.o x86_function.s

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

clean:
	rm -f *.o

