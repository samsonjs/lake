CC = clang
CFLAGS := -Wall -g -I../src
OBJS = ../build/liblake.a
TESTS = test_comment test_dlist test_env test_eval test_fn test_int test_lake

all: $(TESTS)
	@for test in $(TESTS); do \
		echo;             \
		./$$test;         \
	done

test: all

test_comment: laketest.o test_comment.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@

test_dlist: laketest.o test_dlist.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@

test_env: laketest.o test_env.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@

test_eval: laketest.o test_eval.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@

test_fn: laketest.o test_fn.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@

test_int: laketest.o test_int.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@

test_lake: laketest.o test_lake.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@

clean:
	-rm -f *.o $(TESTS)

.PHONY: all $(TESTS)
