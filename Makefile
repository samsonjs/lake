all:
	cd src && make clean && make
	mv src/lake ./lake

clean:
	cd src && make clean

test:
	cd src && make lake.a
	cd test && make

test_clean:
	cd test && make clean

.PHONY: all clean test test_clean
