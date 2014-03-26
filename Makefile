.PHONY: all clean install

ifndef PREFIX
    PREFIX=/usr/local
endif

BIN:=$(DESTDIR)$(PREFIX)/bin
MAN:=$(DESTDIR)$(PREFIX)/man/man1

all: aha

aha: aha.c
	gcc -std=c99 $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) aha.c -o aha

clean:
	rm aha

install: aha
	install -d $(BIN)
	install aha $(BIN)
	install -d $(MAN)
	install aha.1 $(MAN)
