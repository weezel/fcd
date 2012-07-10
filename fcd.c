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
void interactive(long long *);
void usage(void);

int aflag; /* Ask for the choice */
int bflag; /* Bookmark */
int cflag; /* Use curses */
int dflag; /* Maximum word difference in spellcheck */
int iflag; /* Case insensitive search */

int
main(int argc, const char *argv[])
{
	int		 ch;
	size_t		 diff;
	long long	 choice;
	long long	 hits;
	struct resultset rs;
	const char	*dirname;
	const char	*errstr;

	diff = choice = hits = 0;
	memset(&rs, 0, sizeof(struct resultset));

	if (argc < 3)
		usage();

	while ((ch = getopt(argc, (char *const *)argv, "abcd:i")) != -1) {
		switch ((char) ch) {
		case 'a':
			aflag = 1;
			break;
		case 'b':
			bflag = 1;
			break;
		case 'c':
			cflag = 1;
			break;
		case 'd':
			dflag = 1;
			diff = strtonum(optarg, 0, 1000, &errstr);
			if (errstr)
				errx(1, "You fool, %s is not a NUMBER we want! Error: %s",
				     optarg, errstr);
			break;
		case 'i':
			iflag = 1;
			break;
		default:
			usage();
		}
	}
	dirname = argv[optind];

	argc -= optind;
	argv += optind;

	db_init();

	if (cflag)
		process_query(dirname);
	else if (dflag) {
		db_find_spellchecked(TABLE_HOME, dirname, diff);
	}
	if (aflag) {
		db_find_exact(TABLE_HOME, dirname);
		interactive(&choice);
		rs = db_get_choice_by_id(TABLE_HOME, choice);
		print_resultset(&rs);
	}

	db_close();

	return 0;
}

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
interactive(long long *choice)
{
	fprintf(stdout, "\n");
	do {
		fpurge(stdin);
		fprintf(stdout, "Choice > ");
	} while (!scanf("%lld", choice));
}

void
usage(void)
{
	extern char *__progname;

	(void)fprintf(stderr, "usage: %s [-b] [-d] directory name\n", __progname);
	exit(1);
}

