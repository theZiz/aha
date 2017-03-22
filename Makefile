.PHONY: all clean install

PREFIX?=/usr/local

MANDIR?=$(PREFIX)/man
BINMODE?=0755
MANMODE?=644

all: aha

aha: aha.c
	$(CC) -std=c99 $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) aha.c -o aha

clean:
	rm -f aha

install: aha
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m $(BINMODE) aha $(DESTDIR)$(PREFIX)/bin
	install -d $(DESTDIR)$(MANDIR)/man1
	install -m $(MANMODE) aha.1 $(DESTDIR)$(MANDIR)/man1
