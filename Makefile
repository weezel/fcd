CC=clang
#CFLAGS=-pedantic -Wall -Wextra -std=c99 -g
CFLAGS+= -g -Wall -Wextra -pedantic -std=c99
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare
SQLITEFLAGS=-L /usr/local/lib -lsqlite3
CURSESFLAGS=-L/usr/lib -lmenu -lcurses
OS=$(shell uname)

ifeq ($(OS), Linux)
	CFLAGS += -I /usr/local/include/linux -I /usr/lib/gcc/x86_64-linux-gnu/4.6/include-fixed 
	CURSESFLAGS+=-lbsd
endif

ifeq ($(OS), OpenBSD)
	CFLAGS += -I /usr/local/include
endif

.PHONY: all clean

all: spellcheckers dbutils choosewin controller
	${CC} ${CFLAGS} spellcheckers.o dbutils.o \
		choosewin.o controller.o fcd.c -o fcd ${CURSESFLAGS} ${SQLITEFLAGS}
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
clean:
	rm -rf *.o *.core fcd *_test all_tests

