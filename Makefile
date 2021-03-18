############################# Makefile ##########################
all: closest

closest.o: closest.c closest.h
		gcc -o closest.o -c closest.c -W -Wall -pedantic
clean:
		rm -rf *.o
mrproper: clean
		rm -rf pingpong-scheduler