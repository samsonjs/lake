CC = gcc
CFLAGS := -Wall -g -I../src $(shell pkg-config --cflags glib-2.0)
LFLAGS := $(shell pkg-config --libs glib-2.0)
OBJS = ../build/liblake.a
TESTS = test_comment test_dlist test_env test_eval test_fn

all: $(TESTS)
	@clear
	@for test in $(TESTS); do \
		echo;             \
		./$$test;         \
	done

test: all

test_comment: laketest.o test_comment.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ $(OBJS) -o $@

test_dlist: laketest.o test_dlist.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ $(OBJS) -o $@

test_env: laketest.o test_env.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ $(OBJS) -o $@

test_eval: laketest.o test_eval.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ $(OBJS) -o $@

test_fn: laketest.o test_fn.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ $(OBJS) -o $@

clean:
	-rm -f *.o $(TESTS)

.PHONY: all $(TESTS)
