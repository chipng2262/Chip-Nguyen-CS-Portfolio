# Makefile to compile the querier
# Chip Nguyen, COSC 50, Spring 2023

LLIBS = ../libcs50/libcs50-given.a ../common/common.a
TEST_PROG = testing.sh
MAKE = make
FLAGS = -I../libcs50 -I../common

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc

PROGS = querier fuzzquery

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $(FLAGS) $^ $(LLIBS) -o $@

fuzzquery: fuzzquery.o $(LLIBS)
	$(CC) $(CFLAGS) $(FLAGS) $^ $(LLIBS) -o $@

querier.o: querier.c
fuzzquery.o: fuzzquery.c

.PHONY: test valgrind clean all

test: querier $(TEST_PROG) fuzzquery
	bash -v $(TEST_PROG) &> testing.out

valgrind: querier
	$(VALGRIND) ./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "search or home"

all: querier fuzzquery

clean:
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -f querier
	rm -f testing.out