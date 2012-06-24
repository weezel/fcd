#ifndef _DBUTILS_H_
#define _DBUTILS_H_

#define DB_NAME	"test.db"
#define DB_PATH "/home/weezel/ohj/fcd"

int		 dbisopen;
sqlite3		*db;

void db_init(void);
void db_get_choice_by_id(const long long);
size_t db_fuzzy_match_count(const char *);
size_t db_show_matched(const char *);
void insert_item_to_db(struct diritem *);
void db_update_visit_count(const char *);

#endif /* _DBUTILS_H_ */
