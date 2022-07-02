# Makefile for querier
#
# CS50, Spring 2022


#edit for the right programs
PROG = querier fuzzquery
OBJS = querier.o fuzzquery.o
LIBS = ../common/common.a ../libcs50/libcs50.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
VFLAGS = --leak-check=full --show-leak-kinds=all
MAKE = make

querier: querier.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

fuzzquery: fuzzquery.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

.PHONY: all test valgrind clean

all: $(PROG)

test: $(PROG) testing.sh
	./testing.sh

#hardcoded parameters to check memory leaks for querier
valgrind: $(PROG)
	valgrind $(VFLAGS) ./fuzzquery /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-2 10 1 | \
	./querier /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-2 \
	/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-2

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)