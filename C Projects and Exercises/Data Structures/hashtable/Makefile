# Makefile for 'hashtable' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019
# 
# Edited by Chip Nguyen, April 2023

OBJS = hash.o hashtabletest.o hashtable.o ../lib/file.o ../lib/mem.o ../set/set.o
LIBS =

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

hashtabletest: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

hashtabletest.o: hashtable.h ../set/set.h ../lib/file.h
hashtable.o: hashtable.h hash.h ../set/set.h
../set/set.o: ../set/set.h ../lib/mem.h
../lib/file.o: ../lib/file.h
../lib/mem.o: ../lib/mem.h

.PHONY: test valgrind clean

test: hashtabletest
	echo "Testing" | ./hashtabletest

valgrind: hashtabletest
	echo "Detecting memory lead" | $(VALGRIND) ./hashtabletest

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f hashtabletest
	rm -f core