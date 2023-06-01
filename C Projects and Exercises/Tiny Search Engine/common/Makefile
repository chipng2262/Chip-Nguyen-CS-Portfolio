# Makefile to create library of functions and headers in common.a
# Chip Nguyen, COSC 50, Spring 2023

LLIBS = ../libcs50/libcs50-given.a
OBJS = pagedir.o word.o index.o 
LIB = common.a

FLAGS = -DDEBUG=1
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h pagedir.c
word.o: word.c word.h
index.o: index.c index.h index.h ../libcs50/counters.h ../libcs50/file.h ../libcs50/hashtable.h ../libcs50/webpage.h pagedir.h word.h

.PHONY: clean

# clean up after compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o