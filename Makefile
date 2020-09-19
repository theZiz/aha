.PHONY: all clean install

PREFIX?=/usr/local
DATAROOTDIR?=$(PREFIX)/share
MANDIR?=$(DATAROOTDIR)/man

INSTALL?=install -p
BINMODE?=0755
MANMODE?=644

CFLAGS += -std=c99 -Wall -Wextra

all: aha

aha: aha.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) aha.c -o $@

clean:
	rm -f aha

install: aha
	$(INSTALL) -D -m $(BINMODE) aha $(DESTDIR)$(PREFIX)/bin/aha
	$(INSTALL) -D -m $(MANMODE) aha.1 $(DESTDIR)$(MANDIR)/man1/aha.1
