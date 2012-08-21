CC=clang
CFLAGS+= -std=c99 -g -Wall -Wextra -pedantic
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations -Wshadow
CFLAGS+= -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare -Wdeclaration-after-statement
SQLITEFLAGS=-I /usr/local/include -L /usr/local/lib -lsqlite3
CURSESFLAGS=-L/usr/lib -lmenu -lcurses
FILES=choosewin.c  controller.c  dbutils.c  dirindexer.c  fcd.c  spellcheck.c  tests.c

.PHONY: all clean

all: spellcheckers dbutils choosewin controller
	${CC} ${CFLAGS} ${CURSESFLAGS} ${SQLITEFLAGS} spellcheckers.o dbutils.o \
		choosewin.o controller.o fcd.c -o fcd
dbutils:
	${CC} ${CFLAGS} dbutils.c -c -o dbutils.o
controller:
	${CC} ${CFLAGS} -c -o controller.o controller.c
choosewin:
	${CC} ${CFLAGS} -c -o choosewin.o choosewin.c
dirindexer: dbutils
	${CC} ${CFLAGS} dbutils.o dirindexer.c -c -o dirindexer.o
spellcheckers:
	${CC} ${CFLAGS} spellcheck.c -c -o spellcheckers.o
tests: dbutils dirindexer
	#${CC} ${CFLAGS} dbutils.c tests.c -o dbutils_test
	#${CC} ${CFLAGS} ${CURSESFLAGS} ${SQLITEFLAGS} controller.c dbutils.c choosewin.c -o controller_test
	#${CC} ${CFLAGS} ${CURSESFLAGS} choosewin.c -c -o choosewin_test
	${CC} ${CFLAGS} ${SQLITEFLAGS} dbutils.o dirindexer.o tests.c -o all_tests
scanbuild:
	scan-build -analyze-headers -o result_html -v -enable-checker debug.DumpCallGraph make
lint:
	lint -c -e -h -f -I /usr/local/include ${FILES}

clean:
	rm -rf *.o *.core fcd *_test all_tests

