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

void	execute(char *);
int	validate_name(const char *);
int	changedir(const char *);
void	interactive(long long *);
void	usage(void);

int	aflag; /* Ask for the choice, cannot be combined with 'c' flag */
int	bflag; /* Bookmark */
int	cflag; /* Use curses, cannot be combined with 'a' flag */
int	dflag; /* Maximum word difference in spellcheck */
int	Dflag; /* Remove directory item */
int	iflag; /* Case insensitive search */
int	Iflag; /* Insert new directory item */
int	tflag; /* Search for table {ETC, HOME, USR} */

int
main(int argc, const char *argv[])
{
	int		 ch;
	size_t		 diff;
	long long	 choice;
	long long	 hits;
	struct resultset rs;
	char		 fullpath[MAX_CHOICESIZE];
	const char	*dirname;
	const char	*errstr;

	diff = choice = hits = 0;
	memset(&rs, 0, sizeof(struct resultset));
	memset(fullpath, 0, MAX_CHOICESIZE);

	if (argc < 2)
		usage();

	while ((ch = getopt(argc, (char *const *)argv, "abcd:DiIt:")) != -1) {
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
			diff = strtonum(optarg, 0, MAX_HITS, &errstr);
			if (errstr)
				errx(1, "You fool, %s is not a NUMBER we want! Error: %s",
				     optarg, errstr);
			break;
		case 'D':
			Dflag = 1;
			break;
		case 'i':
			iflag = 1;
			break;
		case 'I':
			Iflag = 1;
			break;
		case 't':
			tflag = 1;
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
		process_query(dirname, dflag);
	else if (Iflag || Dflag) {
		int	 rv = 0;
		char	*cwd = NULL;

		if ((cwd = getenv("PWD")) == NULL) {
			warn("Error while retrieving the current path");
			goto error;
		}
		snprintf(cwd, strlen(cwd) + 2, "%s/", cwd);

		if (Iflag && !Dflag) {
			if ((rv = db_insert_dir(TABLE_HOME, cwd, dirname)) != 0) {
				err(1, "Cannot remove directory %s%s from db", cwd, dirname);
				goto error;
			}
		} else if (Dflag) {
			if ((rv = db_delete_dir(TABLE_HOME, cwd, dirname)) == 0) {
				err(1, "Cannot remove directory %s%s from db", cwd, dirname);
				goto error;
			}
		}
	} else if (aflag) {
		if (dflag)
			db_find_spellchecked(TABLE_HOME, dirname, diff);
		else
			db_find_exact(TABLE_HOME, dirname);
		interactive(&choice);
		rs = db_get_choice_by_id(TABLE_HOME, choice);
		print_resultset(&rs);
	} else if (dflag)
		db_find_spellchecked(TABLE_HOME, dirname, diff);
	else
		db_find_exact(TABLE_HOME, dirname);
	snprintf(fullpath, MAX_CHOICESIZE - 1, "%s%s", rs.path, rs.dir);
	fullpath[MAX_CHOICESIZE - 1] = '\0';

error:
	/* Rather close now if we drive to some problem on the next clause */
	db_close();

	execute(fullpath);

	return EXIT_SUCCESS;
}

void
execute(char *dirname)
{
	char	*argv[] = {"sh", "-c", "cd", dirname, NULL};

	execv("/bin/sh", argv);
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

	(void)fprintf(stderr, "usage: %s [-abcdDiIt] directory name\n", __progname);
	exit(1);
}

