# Makefile to compile the crawler
# Chip Nguyen, COSC 50, Spring 2023

LLIBS = ../libcs50/libcs50-given.a ../common/common.a
TEST_PROG = testing.sh
MAKE = makew

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

crawler: crawler.o $(LLIBS)
	$(CC) $(CFLAGS) $(FLAGS) $(LLIBS) $^ -o $@

crawler.o: crawler.c

.PHONY: test valgrind clean

test: crawler $(TEST_PROG) 
	bash $(TEST_PROG) > testing.out

valgrind: crawler $(TEST_PROG) 
	$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 2

clean:
	rm -f core
	rm -f $(LIB) *~ *.o