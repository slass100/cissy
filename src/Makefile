
CC=gcc

OBJS=cissy.o range.o csvline.o csvfield.o

%.o: %.c %.h
	$(CC) -Wall -c -o $@ $<

cissy: $(OBJS)
	$(CC) -Wall -o cissy $(OBJS)

test: test_range test_csvline test_csvfield

test_csvline: test_csvline.c csvline.o csvline.h csvfield.o
		$(CC) -Wall -o test_csvline csvline.o csvfield.o test_csvline.c

test_csvfield: test_csvfield.c csvfield.o csvfield.h
	$(CC) -Wall -o test_csvfield csvfield.o test_csvfield.c

test_range: test_range.c range.o range.h
	$(CC) -Wall -o test_range range.o test_range.c

all: cissy test

install: cissy
	cp cissy $(DESTDIR)/usr/bin/

clean:
	rm -f cissy.exe test_range.exe *.o *.exe.stackdump

