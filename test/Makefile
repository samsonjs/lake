CC = clang
CFLAGS := -Wall -g -I../src
OBJS = ../build/liblake.a
TESTS = test_comment   \
	test_dlist     \
	test_env       \
	test_eval      \
	test_fn        \
	test_int       \
	test_lake      \
	test_list

	# test_parse     \
	# test_primitive \
	# test_str       \
	# test_sym

all: $(TESTS)

test: all

test_comment: laketest.o test_comment.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_comment
	@echo

test_dlist: laketest.o test_dlist.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_dlist
	@echo

test_env: laketest.o test_env.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_env
	@echo

test_eval: laketest.o test_eval.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_eval
	@echo

test_fn: laketest.o test_fn.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_fn
	@echo

test_int: laketest.o test_int.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_int
	@echo

test_lake: laketest.o test_lake.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_lake
	@echo

test_list: laketest.o test_list.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_list
	@echo

test_parse: laketest.o test_parse.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_parse
	@echo

test_primitive: laketest.o test_primitive.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_primitive
	@echo

test_str: laketest.o test_str.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_str
	@echo

test_sym: laketest.o test_sym.o
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@
	@./test_sym
	@echo

clean:
	-rm -f *.o $(TESTS)

.PHONY: all $(TESTS)
