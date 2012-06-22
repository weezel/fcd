#ifndef _DBUTILS_H_
#define _DBUTILS_H_ value

#define TEMPCHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define NUM_CHARS (sizeof(TEMPCHARS) - 1)
#define DB_NAME	"test.db"
#define DB_PATH "/home/weezel/ohj/fcd"
#define KEYLEN 3
/*
 * KEYLEN defines entropy pool. TEMPCHARS length is 62, therefore 62 * 62 * 62
 * = 238328 should be enough for basic usage, hopefully.
 */

sqlite3 *db_open(const char *);
void db_get_choice_by_id(sqlite3 *, const long long);
size_t db_fuzzy_match_count(sqlite3 *, const char *);
size_t db_show_matched(sqlite3 *, const char *);
void db_update_visit_count(sqlite3 *, const char *);
void genkey(char *);

#endif /* _DBUTILS_H_ */
