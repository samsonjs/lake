CC = gcc
CFLAGS := -Wall -g $(shell pkg-config --cflags glib-2.0)
LFLAGS := $(shell pkg-config --libs glib-2.0)
LAKE_OBJS = env.o int.o str.o sym.o parse.o list.o eval.o \
            symtable.o fn.o dlist.o primitive.o comment.o lake.o
OBJS = $(LAKE_OBJS) repl.o

all: lake

lake: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

lake.a: $(LAKE_OBJS)
	rm -f $@
	ar cq $@ $(LAKE_OBJS)

# use touch to prevent errors in case files do not exist
clean:
	@touch dummy.o lake
	rm -f *.o lake
