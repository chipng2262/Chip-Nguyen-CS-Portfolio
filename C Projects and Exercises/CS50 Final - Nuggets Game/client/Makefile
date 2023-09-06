# Makefile for Client
#
# Team Gitters
#

OBJS = client.o ../support/message.o ../support/log.o
LIBS = -lncurses
L = ../support

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L 
CC = gcc
MAKE = make
all: client

client: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o client

client.o: client.c ../support/message.o

.PHONY: test valgrind clean

clean:
	rm -f *~ *.o
	rm -f player
	rm -f core
	rm -f vgcore*