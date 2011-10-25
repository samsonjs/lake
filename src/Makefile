LAKE_BUILD = ../build
CC = clang
CFLAGS := -Wall -g
LAKE_OBJS = $(LAKE_BUILD)/common.o    \
            $(LAKE_BUILD)/comment.o   \
            $(LAKE_BUILD)/bool.o      \
            $(LAKE_BUILD)/dlist.o     \
            $(LAKE_BUILD)/env.o       \
            $(LAKE_BUILD)/eval.o      \
            $(LAKE_BUILD)/fn.o        \
            $(LAKE_BUILD)/hash.o      \
            $(LAKE_BUILD)/int.o       \
            $(LAKE_BUILD)/lake.o      \
            $(LAKE_BUILD)/list.o      \
            $(LAKE_BUILD)/parse.o     \
            $(LAKE_BUILD)/primitive.o \
            $(LAKE_BUILD)/str.o       \
            $(LAKE_BUILD)/sym.o

all: liblake repl

repl: $(LAKE_OBJS) $(LAKE_BUILD)/repl.o
	$(CC) $(CFLAGS) $^ -o $(LAKE_BUILD)/$@

$(LAKE_BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

liblake: $(LAKE_OBJS)
	rm -f $(LAKE_BUILD)/$@.a
	ar cq $(LAKE_BUILD)/$@.a $(LAKE_OBJS)

clean:
	-rm -rf $(LAKE_BUILD)
