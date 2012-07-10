#ifndef _DBUTILS_H_
#define _DBUTILS_H_

#include <sqlite3.h>
#include <stdlib.h>

#include "extern.h"

#define DB_NAME	"test.db"
#define DB_PATH "/home/weezel/ohj/fcd"

struct resultset {
	int bookmark;
	size_t id;
	size_t visits;
	char path[MAXPATHLEN];
	char dir[MAXNAMLEN];
};

int		 dbisopen; // is  > 0 if opened
sqlite3		*db;

void db_init(void);
void db_close(void);
struct resultset db_get_choice_by_id(const char *, const long long);
size_t db_match_count(const char *, const char *);
size_t db_find_spellchecked(const char *, const char *, const size_t);
size_t db_find_exact(const char *, const char *);
void insert_item_to_db(struct diritem *);
int db_update(const char *, struct resultset *, const size_t column);
void print_resultset(struct resultset *);

#endif /* _DBUTILS_H_ */

