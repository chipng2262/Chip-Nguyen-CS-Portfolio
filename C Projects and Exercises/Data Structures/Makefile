# Makefile for CS50 Lab3 
#   Builds and compiles all the data structures.
#
# David Kotz, April 2016, 2017, 2019, 2021

# uncomment the following to turn on verbose memory logging
# (and run `make clean; make` whenever you change this)
# TESTING=-DMEMTEST

# Our Make program and its flags
MAKE = make TESTING=$(TESTING)

# recursively make in each subdirectory
all:
	$(MAKE) --directory=bag
	$(MAKE) --directory=counters
	$(MAKE) --directory=set
	$(MAKE) --directory=hashtable
	$(MAKE) --directory=lib

# 'phony' targets are helpful but do not create any file by that name
.PHONY: clean

# to clean up all derived files
clean:
	rm -f *~
	make --directory=bag clean
	make --directory=set clean
	make --directory=counters clean
	make --directory=hashtable clean
	make --directory=lib clean
