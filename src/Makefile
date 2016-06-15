
CC=gcc

OBJS=cissy.o range.o csvline.o csvfield.o

%.o: %.c %.h
	$(CC) -Wall -c -o $@ $<

cissy: $(OBJS)
	$(CC) -Wall -o cissy $(OBJS)

all: cissy

install: cissy
	mkdir -p $(DESTDIR)/usr/bin
	install -s -m 755 cissy $(DESTDIR)/usr/bin/cissy
	mkdir -p $(DESTDIR)/usr/share/man/man1
	install -m 644 doc/man/cissy.1 $(DESTDIR)/usr/share/man/man1/cissy.1

clean:
	rm -f *.exe *.o *.exe.stackdump cissy

