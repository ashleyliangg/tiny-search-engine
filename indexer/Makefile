# Makefile for indexer
#
# Ashley Liang, CS50 Spring 2022


#edit for the right programs
PROG = indexer indextest
OBJS = indexer.o indextest.o
LIBS = ../common/common.a ../libcs50/libcs50.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
VFLAGS = --leak-check=full --show-leak-kinds=all
MAKE = make

indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

.PHONY: all test valgrind clean

all: $(PROG)

test: $(PROG) testing.sh
	./testing.sh

#hardcoded parameters to check memory leaks for indexer
valgrind: indexer
	valgrind $(VFLAGS) ./indexer /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-2 letters2index

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f *ansr
	rm -f *cpy
	rm -f *index
	rm -f *sorted
	rm -f badfile
	