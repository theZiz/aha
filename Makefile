.PHONY: all clean install

PREFIX?=/usr/local

DATAROOTDIR?=$(PREFIX)/share
MANDIR?=$(DATAROOTDIR)/man

BINMODE?=0755
MANMODE?=644

CFLAGS += -std=c99 -Wall -Wextra

all: aha

aha: aha.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) aha.c -o $@

clean:
	rm -f aha

install: aha
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m $(BINMODE) aha $(DESTDIR)$(PREFIX)/bin
	install -d $(DESTDIR)$(MANDIR)/man1
	install -m $(MANMODE) aha.1 $(DESTDIR)$(MANDIR)/man1
