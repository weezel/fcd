CC=clang
CFLAGS+= -std=c99 -g -Wall -Wextra -pedantic
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare -fdiagnostics-color
SQLITEFLAGS=-L /usr/local/lib -lsqlite3
CURSESFLAGS=-L/usr/lib -lmenu -lcurses
OS=$(shell uname)

ifeq ($(OS), Linux)
	CFLAGS += -I /usr/local/include/linux
	CURSESFLAGS+=-lbsd
endif

ifeq ($(OS), OpenBSD)
	CFLAGS += -I /usr/local/include
endif

.PHONY: all clean

all: spellcheckers dbutils choosewin controller
	${CC} ${CFLAGS} spellcheckers.o dbutils.o \
		choosewin.o controller.o fcd.c -o fcd \
		${CURSESFLAGS} ${SQLITEFLAGS}
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
	${CC} ${CFLAGS} ${SQLITEFLAGS} dbutils.o dirindexer.o tests.c \
		-o all_tests

scanbuild:
	scan-build -analyze-headers -o result_html \
		-v -enable-checker debug.DumpCallGraph make

clean:
	rm -rf *.o *.core fcd *_test all_tests

