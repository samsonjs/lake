LAKE_BUILD = ../build
CC = gcc
CFLAGS := -Wall -g $(shell pkg-config --cflags glib-2.0)
LFLAGS := $(shell pkg-config --libs glib-2.0)
LAKE_OBJS = $(LAKE_BUILD)/comment.o   \
            $(LAKE_BUILD)/dlist.o     \
            $(LAKE_BUILD)/env.o       \
            $(LAKE_BUILD)/eval.o      \
            $(LAKE_BUILD)/fn.o        \
            $(LAKE_BUILD)/int.o       \
            $(LAKE_BUILD)/lake.o      \
            $(LAKE_BUILD)/list.o      \
            $(LAKE_BUILD)/parse.o     \
            $(LAKE_BUILD)/primitive.o \
            $(LAKE_BUILD)/str.o       \
            $(LAKE_BUILD)/sym.o       \
            $(LAKE_BUILD)/symtable.o

all: lake repl

repl: $(LAKE_OBJS) $(LAKE_BUILD)/repl.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $(LAKE_BUILD)/$@

$(LAKE_BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

lake: $(LAKE_OBJS)
	rm -f $(LAKE_BUILD)/$@.a
	ar cq $(LAKE_BUILD)/$@.a $(LAKE_OBJS)

clean:
	-rm -rf $(LAKE_BUILD)
