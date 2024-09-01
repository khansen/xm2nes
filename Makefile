INSTALL = install
CFLAGS = -Wall -g
LFLAGS =
OBJS = xm2gba.o xm.o main.o

prefix = /usr/local
datarootdir = $(prefix)/share
datadir = $(datarootdir)
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
infodir = $(datarootdir)/info
mandir = $(datarootdir)/man
docbookxsldir = /sw/share/xml/xsl/docbook-xsl

xm2gba: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o xm2gba

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: xm2gba
	$(INSTALL) -m 0755 xm2gba $(bindir)
	$(INSTALL) -m 0444 xm2gba.1 $(mandir)/man1

doc: xm2gba-refentry.docbook
	xsltproc $(docbookxsldir)/manpages/docbook.xsl $<
	xsltproc $(docbookxsldir)/html/docbook.xsl $< > doc/index.html

clean:
	rm -f $(OBJS) xm2gba xm2gba.exe

.PHONY: clean install
