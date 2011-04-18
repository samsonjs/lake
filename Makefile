CC=gcc -Wall -ansi -G

all: lake

lake: lake.o env.o hashtab.o int.o string.o sym.o parse.o bool.o list.o

clean:
	rm *.o lake