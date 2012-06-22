#include <menu.h>
#include <sqlite3.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "extern.h"
#include "choosewin.h"
#include "dbutils.h"

#include "controller.h"


void
alloc_dlist(size_t n)
{
	if ((dirlist = calloc(n, sizeof(struct dlist))) == NULL)
		err(1, "malloc failed badly");
}

void
free_dlist(void)
{
	free(dirlist);
	dirlist = NULL;
}


void
process_query(const char *dirname)
{
	size_t		  found;
	char		  choice[MAX_CHOICESIZE];
	MENU		 *choices = NULL;

	if (dbisopen < 1)
		db_init();
	found = db_fuzzy_match_count(dirname);
	if (found > MAX_HITS) {
		found = MAX_HITS;
		hit_max_items = 1;
	}

	alloc_dlist(found); /* must be before combine_items() call */
	choices = combine_items(dirname, found);

	init_screen();
	show_chooser_win(choices, found, choice);

	free_menu_and_items(choices);
	free_dlist();
	endwin();

	sqlite3_close(db);
	sqlite3_free(db);

	fprintf(stdout, "%s\n", choice);
}

/*
 * Insert found database diritems into curses menu
 */
MENU *
combine_items(const char *dirname, size_t item_count)
{
	int		  rc;
	size_t		  hits;
	char		 *q;
	sqlite3_stmt	 *stmt = NULL;
	ITEM		**items = NULL;
	MENU		 *m = NULL;

	hits = rc = 0;

	if ((items = (ITEM **)calloc(item_count + 1, sizeof(ITEM *))) == NULL)
		err(1, "controller: malloc");

	q = sqlite3_mprintf("SELECT path, dir FROM homedir WHERE dir LIKE '%%%q%%';", dirname);
	if ((rc = sqlite3_prepare_v2(db, q, strlen(q), &stmt, NULL)))
		fprintf(stderr, "Error while preparing %s\n", sqlite3_errmsg(db));

	for (hits = 0; hits < item_count && sqlite3_step(stmt) == SQLITE_ROW;) {
		char	*pathtmp;
		char	*dnametmp;

		pathtmp = (char *)sqlite3_column_text(stmt, 0);
		dnametmp = (char *)sqlite3_column_text(stmt, 1);
		strlcpy(dirlist[hits].abspath, pathtmp, MAXPATHLEN);
		strlcpy(dirlist[hits].dname, dnametmp, MAXNAMLEN);
		if ((pathtmp != NULL) && (dnametmp != NULL)) {
			items[hits] = new_item(dirlist[hits].abspath, dirlist[hits].dname);
		}

		/* TODO spellcheck
		if (dflag > 0) {
			size_t difference = spellcheck(word1, sqlite3_columnt, dflag);
			if (difference < dlfag)
				hits++;
		}
		*/
		hits++;
	}
	/* terminate the menu */
	if (hits + 1 > item_count)
		hits = item_count;
	items[hits] = (ITEM *)NULL;

	m = new_menu((ITEM **)items);

	if (q)
		sqlite3_free(q);
	sqlite3_finalize(stmt);

	return m;
}

int
main(int argc, const char *argv[])
{
	process_query((argc < 3) ? "t" : argv[1]);

	return 0;
}

