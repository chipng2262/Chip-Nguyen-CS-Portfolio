# Makefile for shared module
#
# Team Gitters
#
# CS50 May 2023

OBJS = grid.o gridPoint.o map.o player.o game.o
LLIBS = ../libcs50/libcs50-given.a shared.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I ../libcs50
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: $(LLIBS)

$(LLIBS): $(OBJS)
	ar cr $@ $^ -lm

grid: grid.o

gridPoint: gridPoint.o

map: map.o

game: game.o

constants: constants.o

player: player.o

grid.o: grid.c grid.h

gridPoint.o: gridPoint.c gridPoint.h

map.o: map.c map.h

player.o: player.c player.h

game.o: game.c game.h

constants.o: constants.h

clean:
	rm -f *~ *.o
	rm -f server
	rm -f core
	rm -f vgcore*