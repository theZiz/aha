.PHONY: all clean install

PREFIX?=/usr/local

BIN:=$(DESTDIR)$(PREFIX)/bin
MAN:=$(DESTDIR)$(PREFIX)/man/man1

all: aha

aha: aha.c
	gcc -std=c99 $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) aha.c -o aha

clean:
	rm -f aha

install: aha
	install -d $(BIN)
	install aha $(BIN)
	install -d $(MAN)
	install aha.1 $(MAN)
