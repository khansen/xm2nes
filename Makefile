INSTALL = install
CFLAGS = -Wall -g
LFLAGS =
OBJS = xm2gb.o xm.o main.o

prefix = /usr/local
datarootdir = $(prefix)/share
datadir = $(datarootdir)
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
infodir = $(datarootdir)/info
mandir = $(datarootdir)/man
docbookxsldir = /sw/share/xml/xsl/docbook-xsl

xm2gb: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o xm2gb

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: xm2gb
	$(INSTALL) -m 0755 xm2gb $(bindir)
	$(INSTALL) -m 0444 xm2gb.1 $(mandir)/man1

doc: xm2nes-refentry.docbook
	xsltproc $(docbookxsldir)/manpages/docbook.xsl $<
	xsltproc $(docbookxsldir)/html/docbook.xsl $< > doc/index.html

clean:
	rm -f $(OBJS) xm2gb xm2gb.exe

.PHONY: clean install
