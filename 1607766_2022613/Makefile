############################# Makefile ##########################
all: closest

closest: closest.o -lm

closest.o: closest.c closest.h
		gcc -o closest.o -c closest.c -lm -W -Wall -pedantic
clean:
		rm -rf *.o
mrproper: clean
		rm -rf closest
