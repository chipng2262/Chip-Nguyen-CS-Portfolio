# Makefile for 'lib' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016

# PROG =
OBJS = mem.o file.o
LIBS =

# uncomment the following to turn on verbose memory logging
# (comment or uncomment the following, then "make clean; make")
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING)
MAKE = make
CC = gcc

default: $(OBJS)

mem.o: mem.h

file.o: file.h

.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
