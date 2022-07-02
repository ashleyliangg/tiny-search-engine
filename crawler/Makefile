# Makefile for crawler
#
# CS50, Spring 2022


#edit for the right programs
PROG = crawler
OBJS = crawler.o
LIBS = ../common/common.a ../libcs50/libcs50.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
VFLAGS = --leak-check=full --show-leak-kinds=all
MAKE = make

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ 
# $(LIBS) -o $@

.PHONY: test valgrind clean

test: $(PROG) testing.sh
	mkdir -p testdir
	mkdir -p index0
	mkdir -p index1
	mkdir -p index2
	mkdir -p index3
	mkdir -p index4
	mkdir -p index5
	mkdir -p indexE
	mkdir -p toscrape
	mkdir -p wiki0
	mkdir -p wiki1
	mkdir -p wiki2
	mkdir -p wikidart
	./testing.sh

#hardcoded parameters
valgrind: $(PROG)
	mkdir -p testdir
	valgrind $(VFLAGS) ./$(PROG) http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testdir 2

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	if [ -d "testdir" ]; then \
		rm -r testdir; \
	fi

	if [ -d "index0" ]; then \
		rm -r index0; \
	fi

	if [ -d "index1" ]; then \
		rm -r index1; \
	fi

	if [ -d "index2" ]; then \
		rm -r index2; \
	fi

	if [ -d "index3" ]; then \
		rm -r index3; \
	fi

	if [ -d "index4" ]; then \
		rm -r index4; \
	fi

	if [ -d "index5" ]; then \
		rm -r index5; \
	fi

	if [ -d "toscrape" ]; then \
		rm -r toscrape; \
	fi

	if [ -d "wiki0" ]; then \
		rm -r wiki0; \
	fi

	if [ -d "wiki1" ]; then \
		rm -r wiki1; \
	fi

	if [ -d "wiki2" ]; then \
		rm -r wiki2; \
	fi

	if [ -d "wikidart" ]; then \
		rm -r wikidart; \
	fi