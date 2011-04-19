CC = gcc
CFLAGS := -Wall -g $(shell pkg-config --cflags glib-2.0)
LFLAGS := $(shell pkg-config --libs glib-2.0)

all: lake

lake: lake.o env.o int.o string.o sym.o parse.o bool.o list.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

# use touch to prevent errors in case files do not exist
clean:
	@touch dummy.o lake
	rm -f *.o lake
