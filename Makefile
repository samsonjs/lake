all:
	cd src && make clean && make
	mv src/lake ./lake

clean:
	cd src && make clean
