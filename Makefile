
all:
	cd src; make all
	cd test; make all

install:
	cd src; make install

clean:
	cd src; make clean
	cd test; make clean
