# Makefile to compile the crawler
# Chip Nguyen, COSC 50, Spring 2023

LLIBS = ../libcs50/libcs50-given.a ../common/common.a
TEST_PROG = testing.sh
MAKE = make
FLAGS = -I../libcs50 -I../common

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc

PROGS = indexer indextest

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $(FLAGS) $^ $(LLIBS) -o $@

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $(FLAGS) $^ $(LLIBS) -o $@

indexer.o: indexer.c

.PHONY: test valgrind clean

all: indexer indextest

test: indexer indextest $(TEST_PROG) 
	bash $(TEST_PROG) > testing.out

valgrind: indexer indextest
	$(VALGRIND) ./indexer ../data/letters-10 ../data/letters10.index.valgrind
	$(VALGRIND) ./indextest ../data/letters10.index.valgrind ../data/letters10.inverted.valgrind

clean:
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -f indexer