CC=clang
CFLAGS=-ansi -pedantic -Wall -std=c99 -g
SQLITEFLAGS=-I /usr/local/include -L /usr/local/lib -lsqlite3
CURSESFLAGS=-L/usr/lib -lmenu -lcurses

.PHONY: clean

#all: choosewin dbutils
all: choosewin dbutils controller
	${CC} ${CFLAGS} ${CURSESFLAGS} ${SQLITEFLAGS} dbutils.o choosewin.o controller.o fcd.c -o fcd
dbutils:
	#${CC} ${CFLAGS} dbutils.c test_db.c -o dbutils_test
	${CC} ${CFLAGS} -c dbutils.c -o dbutils.o
controller:
	${CC} ${CFLAGS} ${CURSESFLAGS} ${SQLITEFLAGS} controller.c dbutils.c choosewin.c -o controller_test
	#${CC} ${CFLAGS} -c -o controller.o controller.c
choosewin:
	${CC} ${CFLAGS} ${CURSESFLAGS} choosewin.c -o choosewin_test
	#${CC} ${CFLAGS} -c -o choosewin.o choosewin.c
dirindexer: dbutils
	${CC} ${CFLAGS} ${SQLITEFLAGS} dbutils.o dirindexer.c test_db.c -o dirindexer_test
clean:
	rm -rf *.o *.core fcd *_test

