BINDIR =	/usr/local/bin
MANDIR =	/usr/local/man/man1
CC =		gcc
CFLAGS =	-O
LDFLAGS =	-s

all:		weblog_parse

weblog_parse:	weblog_parse.c
	${CC} ${CFLAGS} weblog_parse.c ${LDFLAGS} -o weblog_parse

install:	all
	rm -f ${BINDIR}/weblog_parse
	cp weblog_parse ${BINDIR}
	rm -f ${MANDIR}/weblog_parse.1
	cp weblog_parse.1 ${MANDIR}

clean:
	rm -f weblog_parse *.o core
