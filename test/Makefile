
CC=gcc

OBJS=../src/range.o ../src/csvline.o ../src/csvfield.o

INCLUDE=../src

%.o: %.c %.h
	$(CC) -Wall -c -o $@ $<

test: test_range test_csvline test_csvfield

test_csvline: test_csvline.c ../src/csvline.o ../src/csvline.h ../src/csvfield.o ../src/csvfield.h
	$(CC) -Wall -I $(INCLUDE) -o test_csvline ../src/csvline.o ../src/csvfield.o test_csvline.c

test_csvfield: test_csvfield.c ../src/csvfield.o ../src/csvfield.h
	$(CC) -Wall  -I $(INCLUDE) -o test_csvfield ../src/csvfield.o test_csvfield.c

test_range: test_range.c ../src/range.o ../src/range.h
	$(CC) -Wall  -I $(INCLUDE) -o test_range ../src/range.o test_range.c

all: test

clean:
	rm -f *.exe *.o *.exe.stackdump test_csvline test_csvfield test_range

