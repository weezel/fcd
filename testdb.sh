#!/bin/sh

if [ -f test.db ]; then
	rm -f test.db
fi

sqlite3 test.db "create table homedir(
			id INTEGER PRIMARY KEY,
			path VARCHAR(1024),
			dir VARCHAR(128),
			visits INT,
			bookmark INT,
			UNIQUE(path, dir));" #&&
#sqlite3 test.db "insert into homedir values(NULL, '/home/weezel/', 'ohj', 5, 1);" &&
#sqlite3 test.db "insert into homedir values(NULL, '/home/weezel/', 'snapshots', 2, 0);" &&
#sqlite3 test.db "insert into homedir values(NULL, '/home/weezel/', 'torrent', 1, 0);" &&
#sqlite3 test.db "insert into homedir values(NULL, '/home/weezel/', 'security', 3, 1);"

