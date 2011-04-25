all:
	cd src && make all

liblake:
	cd src && make liblake

repl:
	cd src && make repl

clean:
	cd src && make clean
	-rm -f lake

test:
	cd src && make liblake
	cd test && make

test_clean:
	cd test && make clean

.PHONY: all clean test test_clean
