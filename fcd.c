#include <sqlite3.h>

#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslimits.h>
#include <unistd.h>

#include "extern.h"
#include "dbutils.h"
#include "choosewin.h"
#include "controller.h"

int validate_name(const char *);
int changedir(const char *);
void interactive(void);
void usage(void);

int
main(int argc, const char *argv[])
{
	int		 lflag, iflag, ch;
	size_t		 choice = 0;
	long long	 hits;
	const char	*dirname;

	hits = lflag = 0;

	if (argc < 2)
		usage();

	while ((ch = getopt(argc, (char *const *)argv, "bdi")) != -1) {
		switch ((char) ch) {
		case 'b': /* Bookmark */
			lflag = 1;
			break;
		case 'd': /* Max word difference */
			break;
		case 'i': /* Interactive */
			iflag = 1;
			break;
		default: /* No extra options */
			usage();
		}
	}
	dirname = argv[optind];

	argc -= optind;
	argv += optind;

	process_query(dirname);
	if (iflag)
		interactive(&choice);

	/*
	db = db_open(DB_NAME);
	hits = db_show_matched(db, dirname);

	sqlite3_close(db);
	sqlite3_free(db);
	*/

	return 0;
}

/* if str is valid, return 0, else -1 */
int
validate_name(const char *str)
{
	char	*cp = '\0';

	cp = (char *)&str[0];
	while (*cp != '\0') {
		if (isalnum(*cp) == 0)
			return -1;
		cp++;
	}

	return 0;
}

void
interactive(size_t *choice)
{
	do {
		fpurge(stdin);
		fprintf(stdout, "Choice > ");
	} while (!scanf("%d", &choice));
}

void
usage(void)
{
	extern char *__progname;

	(void)fprintf(stderr, "usage: %s [-b] [-d] [name]\n", __progname);
	exit(1);
}

