############################# Makefile ##########################
all: tsp

closest: tsp.o -lm

closest.o: tsp.c tsp.h
		gcc -o tsp.o -c tsp.c -lm -W -Wall -pedantic
clean:
		rm -rf *.o
mrproper: clean
		rm -rf tsp
