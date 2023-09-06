OBJS = server.o ../support/message.o ../support/log.o
LIBS = -lncurses ../shared/shared.a ../libcs50/libcs50-given.a -lm
L = ../support

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$(L)
CC = gcc
MAKE = make

all: server

server: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o server

server.o: server.c ../shared/shared.a
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: test valgrind clean

clean:
	rm -f *~ *.o
	rm -f server
	rm -f core
	rm -f vgcore*