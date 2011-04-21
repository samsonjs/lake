CC = gcc
CFLAGS := -Wall -g $(shell pkg-config --cflags glib-2.0)
LFLAGS := $(shell pkg-config --libs glib-2.0)
OBJS = lake.o env.o int.o string.o sym.o parse.o bool.o list.o eval.o \
       symtable.o fn.o dlist.o primitive.o comment.o

all: lake

lake: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

# use touch to prevent errors in case files do not exist
clean:
	@touch dummy.o lake
	rm -f *.o lake
