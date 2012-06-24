CC=clang
CFLAGS=-ansi -pedantic -Wall -std=c99 -g
SQLITEFLAGS=-I /usr/local/include -L /usr/local/lib -lsqlite3
CURSESFLAGS=-L/usr/lib -lmenu -lcurses

#all: choosewin dbutils
all: choosewin dbutils controller
	${CC} ${CFLAGS} ${CURSESFLAGS} ${SQLITEFLAGS} dbutils.o choosewin.o controller.o fcd.c -o fcd
dbutils:
	${CC} ${CFLAGS} -c -o dbutils.o dbutils.c
controller:
	${CC} ${CFLAGS} ${CURSESFLAGS} ${SQLITEFLAGS} -o controller controller.c dbutils.c choosewin.c
	#${CC} ${CFLAGS} -c -o controller.o controller.c
choosewin:
	${CC} ${CFLAGS} ${CURSESFLAGS} -o choosewin choosewin.c
	#${CC} ${CFLAGS} -c -o choosewin.o choosewin.c
dirindexer: dbutils
	${CC} ${CFLAGS} ${SQLITEFLAGS} dirindexer.c dbutils.o -o dirindexer
clean:
	rm -rf *.o *.core fcd choosewin controller dirindexer

.PHONY: clean

