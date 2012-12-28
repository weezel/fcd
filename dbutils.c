#include <sqlite3.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined __linux__
#include <bsd/string.h>
#endif

#include "extern.h"
#include "dbutils.h"

extern size_t levenstein_distance(const char *, const size_t,
				  const char *, const size_t);

void
db_init(void)
{
	int	 rc;
	int	 rv;
	char	*db_path;
	char	*db_name;

	db_path = DB_PATH;
	db_name = DB_NAME;

	if ((rv = chdir(db_path)) != 0)
		err(1, "Cannot change directory to: %s", DB_PATH);

	if ((rc = sqlite3_open(db_name, &db)) != 0) {
		if (db)
			sqlite3_close(db);
		err(1, "Cannot open database %s", sqlite3_errmsg(db));
	}
	dbisopen = 1;
}

void
db_close(void)
{
	if (db) {
		sqlite3_close(db);
		sqlite3_free(db);
	}
}

struct resultset
db_get_choice_by_id(const char *table, const long long id)
{
	int		 rc;
	char		*q;
	sqlite3_stmt	*stmt;
	struct resultset rs;

	memset(&rs, 0, sizeof(struct resultset));

	q = sqlite3_mprintf("SELECT id, path, dir, visits, bookmark FROM %q WHERE id = ?;", table);

	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL))) {
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));
		goto sqlerror;
	}

	sqlite3_bind_int64(stmt, 1, id);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		size_t	 path_len = 0;
		size_t	 dir_len  = 0;
		char	*pathname = (char *) sqlite3_column_text(stmt, 1);
		char	*dirname  = (char *) sqlite3_column_text(stmt, 2);

		path_len = strlen(pathname) + 1;
		dir_len  = strlen(dirname)  + 1;

		strlcpy(rs.path, pathname, (path_len > MAXPATHLEN) ? MAXPATHLEN : path_len);
		strlcpy(rs.dir,  dirname,  (dir_len  > NAME_MAX)  ? NAME_MAX : dir_len);
		rs.id = sqlite3_column_int64(stmt, 0);
		rs.visits = sqlite3_column_int64(stmt, 3);
		rs.bookmark = sqlite3_column_int64(stmt, 4);
	}

sqlerror:
	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return rs;
}

size_t
db_match_count(const char *table, const char *path, const char *dirname)
{
	int		 rc;
	size_t		 hits;
	char		*q;
	sqlite3_stmt	*stmt;

	hits = 0;

	if (path == NULL)
		q = sqlite3_mprintf("SELECT COUNT(dir) FROM %q WHERE dir LIKE '%q%%';", table, dirname);
	else
		q = sqlite3_mprintf("SELECT COUNT(dir) FROM %q WHERE path LIKE '%q%%' AND dir LIKE '%q%%';", table, path, dirname);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL))) {
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));
		goto error;
	}

	sqlite3_step(stmt);
	hits = (size_t) sqlite3_column_int64(stmt, 0);

error:
	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return hits;
}

size_t
db_find_spellchecked(const char *table, const char *dirname, const size_t diff)
{
	int		 rc;
	size_t		 hits;
	char		*q;
	sqlite3_stmt	*stmt;

	hits = 0;

	q = sqlite3_mprintf("SELECT id, path, dir FROM %q", table);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL))) {
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));
		goto error;
	}

	fprintf(stdout, "ID%-5sPATH%-39sDIRECTORY%-12s(DIFF, VISITS, BOOKMARKED)\n",
		"", "", "");
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		const char *tablerow = (const char *) sqlite3_column_text(stmt, 2);
		size_t wdiff = levenstein_distance(dirname,
						   (size_t) strlen(dirname),
						   tablerow,
						   (size_t) strlen(tablerow));

		if (wdiff <= diff)
			fprintf(stdout, "%-6llu %s   %s (%zu, %llu, %llu)\n",
				sqlite3_column_int64(stmt, 0),
				sqlite3_column_text(stmt, 1),
				sqlite3_column_text(stmt, 2),
				wdiff,
				sqlite3_column_int64(stmt, 3),
				sqlite3_column_int64(stmt, 4));
	}

error:
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

	q = sqlite3_mprintf("SELECT * FROM %q WHERE dir LIKE '%q%%';", table, dirname);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));

	fprintf(stdout, "ID%-5sPATH%-39sDIRECTORY%-12sVISITS%-2sBOOKMARKED\n", "", "", "", "");
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		fprintf(stdout, "%-6llu %-42s %-20s %-7llu %llu\n", sqlite3_column_int64(stmt, 0),
			sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2),
			sqlite3_column_int64(stmt, 3), sqlite3_column_int64(stmt, 4));
		hits++;
	}

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return hits;
}

int
db_insert_diritem(const char *table, struct diritem *di)
{
	int		 rc;
	char		*q;
	sqlite3_stmt	*stmt;

	q = sqlite3_mprintf("INSERT INTO %q(path, dir, visits, bookmark) "
			    "VALUES('%q', '%q', %q, %q);",
			    table, di->path, di->dname, 0, 0);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n",
			sqlite3_errmsg(db));

	if ((rc = sqlite3_step(stmt)) != SQLITE_OK)
		fprintf(stderr, "Error while adding to db: %s\n",
			sqlite3_errmsg(db));

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return rc;
}

int
db_update(const char *table, struct resultset *rs, const size_t column)
{
	int		 rtrnval = -1;
	int		 rc;
	char		*q;
	sqlite3_stmt	*stmt;

	switch (column) {
	case BOOKMARK:
		rs->bookmark ^= 1;
		q = sqlite3_mprintf("UPDATE %q SET bookmark %q "
				    "WHERE dir LIKE '%q';",
				    table, rs->bookmark);
		break;
	case VISIT:
		q = sqlite3_mprintf("UPDATE %q SET visits %q "
				    "WHERE dir LIKE '%q';",
				    table, ++rs->visits);
		break;
	default:
		fprintf(stderr, "You brought some dirty garbage! "
				"%lu ain't good value.\n", column);
		return rtrnval;
	}
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n",
			sqlite3_errmsg(db));

	rtrnval = sqlite3_step(stmt);

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return rtrnval;
}

int
db_insert_dir(const char *table, const char *path, const char *dirname)
{
	int		 rc = 0;
	size_t		 prevmatches = 0;
	char		*q;
	sqlite3_stmt	*stmt;

	prevmatches = db_match_count(TABLE_HOME, path, dirname);
	if (prevmatches > 0)
		fprintf(stdout, "DB already contains %s%s: %zu\n", path, dirname, prevmatches);

	q = sqlite3_mprintf("INSERT INTO %q(path, dir, visits, bookmark) "
			    "VALUES('%q', '%q', %q, %q);",
			    table, path, dirname, 0, 0);
	fprintf(stdout, "SQL: %s\n", q);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n",
			sqlite3_errmsg(db));

	rc = 0;
	if ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
		fprintf(stdout, "RC WAS: %d\n", rc);
		fprintf(stderr, "Error in insert: %s\n",
			sqlite3_errmsg(db));
		rc = -1;
	}

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return rc;
}

int
db_delete_dir(const char *table, const char *path, const char *dirname)
{
	int		 rm_count = 0;
	int		 rc = 0;
	char		*q;
	sqlite3_stmt	*stmt;

	q = sqlite3_mprintf("DELETE FROM %q WHERE path LIKE '%q' AND "
			    "dir LIKE '%q';", table, path, dirname);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n",
			sqlite3_errmsg(db));

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		fprintf(stdout, "Deleting: %s/%s\n", path, dirname);
		rm_count++;
	}

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return rm_count;
}

void
print_resultset(struct resultset *rs)
{
	fprintf(stdout, "\n%-49s ", rs->path);
	fprintf(stdout, "<< %s >> ", rs->dir);
	fprintf(stdout, "[%zu, %zu, %d]\n", rs->id, rs->visits, rs->bookmark);
}

