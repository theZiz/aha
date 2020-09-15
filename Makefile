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
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) -m $(BINMODE) aha $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) -d $(DESTDIR)$(MANDIR)/man1
	$(INSTALL) -m $(MANMODE) aha.1 $(DESTDIR)$(MANDIR)/man1
