#ifndef _DBUTILS_H_
#define _DBUTILS_H_

#include <sqlite3.h>
#include <stdlib.h>

#include "extern.h"

#define DB_NAME	"test.db"
#define DB_PATH "/home/weezel/ohj/fcd"

struct resultset {
	int primarykey;
	int bookmark;
	size_t visits;
	char path[MAXPATHLEN];
	char dir[MAXNAMLEN];
};

int		 dbisopen; // is  > 0 if opened
sqlite3		*db;

void db_init(void);
void db_get_choice_by_id(const long long);
size_t db_match_count(const char *);
size_t db_show_matched(const char *);
size_t db_find_exact(const char *, const char *);
void insert_item_to_db(struct diritem *);
void db_update_visit_count(const char *, const char *);

#endif /* _DBUTILS_H_ */
