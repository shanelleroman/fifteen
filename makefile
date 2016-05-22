# make file for fifteen.c
CC=gcc
CFLAGS=-std=c99 -Wall -g3

fifteen: fifteen.o godmode.o
	$(CC) $(CFLAGS) -o $@ $^

# rebuild make file whenever source file changes
fifteen.o: fifteen.c godmode.h
godmode.o: godmode.c godmode.h

clean: 
	$(RM) fifteen *.o *~