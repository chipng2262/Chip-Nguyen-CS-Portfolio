# Makefile
# Nuggets - gitters 

.PHONY: all clean

all:
	make -C shared
	make -C server
	make -C client

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	make -C shared clean
	make -C server clean
	make -C client clean