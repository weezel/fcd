#include <sqlite3.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "extern.h"
#include "dbutils.h"

void
db_init(void)
{
	int	rc;

	if ((rc = sqlite3_open(DB_NAME, &db)) != 0) {
		err(1, "Cannot open database %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(5);
	}
	dbisopen = 1;
}

void
db_get_choice_by_id(const long long choice)
{
	int		 rc;
	sqlite3_stmt	*stmt;

	if ((rc = sqlite3_prepare_v2(db, "SELECT path, dir FROM homedir WHERE id = ?;", -1, &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));

	sqlite3_bind_int64(stmt, 1, choice);

	if (sqlite3_step(stmt) == SQLITE_ROW)
		fprintf(stdout, "%s%s\n", sqlite3_column_text(stmt, 0),
			sqlite3_column_text(stmt, 1));

	sqlite3_finalize(stmt);
}

size_t
db_match_count(const char *dirname)
{
	int		 rc;
	size_t		 hits;
	char		*q;
	sqlite3_stmt	*stmt;

	hits = 0;

	q = sqlite3_mprintf("SELECT COUNT(dir) FROM homedir WHERE dir LIKE '%%%q%%';", dirname);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));

	sqlite3_step(stmt);
	hits = (size_t) sqlite3_column_int64(stmt, 0);

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return hits;
}

size_t
db_show_matched(const char *name)
{
	int		 rc;
	size_t		 hits;
	char		*q;
	sqlite3_stmt	*stmt;

	hits = 0;

	q = sqlite3_mprintf("SELECT id, path, dir FROM homedir WHERE dir LIKE '%%%q%%';", name);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		fprintf(stdout, "%llu%-5s%s  %s\n", sqlite3_column_int64(stmt, 0), "",
			sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2));
		/*
		*singlematch = sqlite3_column_int64(stmt, 0);
		XXX
		if (dflag > 0) {
			size_t difference = spellcheck(word1, sqlite3_columnt, dflag);
			if (difference < dlfag)
				hits++;
		}
		*/
		hits++;
	}

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return hits;
}

size_t
db_find_exact(const char *table, const char *dirname)
{
	int		 rc;
	size_t		 hits;
	char		*q;
	sqlite3_stmt	*stmt;

	hits = 0;

	q = sqlite3_mprintf("SELECT id, path, dir FROM %q WHERE dir LIKE '%q';", table, dirname);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		fprintf(stdout, "%llu%-5s%s  %s\n", sqlite3_column_int64(stmt, 0), "",
			sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2));
		hits++;
	}

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return hits;
}

void
insert_item_to_db(struct diritem *di)
{
	int		 rc;
	char		*q;
	sqlite3_stmt	*stmt;

	q = sqlite3_mprintf("INSERT INTO homedir(path, dir, visits, bookmark) VALUES('%q', '%q', %q, %q);",
			    di->path, di->dname, 0, 0);
	#ifdef DEBUG
	fprintf(stdout, "SQL-SYNTAX: %s\n", q);
	#endif
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));

	//sqlite3_exec(db, zSQL, 0, 0, 0);

	sqlite3_step(stmt);

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);
}

void
db_update_visit_count(const char *s)
{
}

