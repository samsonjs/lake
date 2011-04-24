CC = gcc
CFLAGS := -Wall -g -I../src $(shell pkg-config --cflags glib-2.0)
LFLAGS := $(shell pkg-config --libs glib-2.0)
DEPS = minunit.h ../src/lake.a
TESTS = test_comment test_dlist test_env

all: $(TESTS)
	@clear
	@for test in $(TESTS); do \
		echo;             \
		./$$test;         \
	done

test_comment: $(DEPS) test_comment.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

test_dlist: $(DEPS) test_dlist.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

test_env: $(DEPS) test_env.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

test_eval: $(DEPS) test_eval.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

# use touch to prevent errors in case files do not exist
clean:
	@touch dummy.o $(TESTS)
	rm -f *.o $(TESTS)

.PHONY: all $(TESTS)
