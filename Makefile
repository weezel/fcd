CC=clang
CFLAGS=-ansi -pedantic -Wall -std=c99 -g
SQLITEFLAGS=-I /usr/local/include -L /usr/local/lib -lsqlite3
CURSESFLAGS=-L/usr/lib -lmenu -lcurses

.PHONY: clean

#all: choosewin dbutils
all: dbutils choosewin controller
	${CC} ${CFLAGS} ${CURSESFLAGS} ${SQLITEFLAGS} dbutils.o choosewin.o controller.o fcd.c -o fcd
dbutils:
	#${CC} ${CFLAGS} dbutils.c test_db.c -o dbutils_test
	${CC} ${CFLAGS} dbutils.c -c -o dbutils.o
controller:
	${CC} ${CFLAGS} ${CURSESFLAGS} ${SQLITEFLAGS} controller.c dbutils.c choosewin.c -o controller_test
	#${CC} ${CFLAGS} -c -o controller.o controller.c
choosewin:
	${CC} ${CFLAGS} ${CURSESFLAGS} choosewin.c -c -o choosewin_test
	#${CC} ${CFLAGS} -c -o choosewin.o choosewin.c
dirindexer: dbutils
	${CC} ${CFLAGS} dbutils.o dirindexer.c -c -o dirindexer.o
tests: dbutils dirindexer
	${CC} ${CFLAGS} ${SQLITEFLAGS} dbutils.o dirindexer.o test_db.c -o all_tests
clean:
	rm -rf *.o *.core fcd *_test all_tests

