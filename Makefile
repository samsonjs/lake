all:
	cd src && make all

lake:
	cd src && make lake

repl:
	cd src && make repl

clean:
	cd src && make clean
	-rm -f lake

test:
	cd src && make lake
	cd test && make

test_clean:
	cd test && make clean

.PHONY: all clean test test_clean
