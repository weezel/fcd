#include <sqlite3.h>

#if defined __OpenBSD__
#include <sys/syslimits.h>
#endif
#if defined __linux__
#include <sys/types.h>
#ifndef __u_char_defined
typedef __u_char	u_char;
typedef __u_short	u_short;
typedef __u_int		u_int;
typedef __u_long	u_long;
# define __u_char_defined
#endif
#include <getopt.h>
#include <bsd/stdlib.h>
#endif

#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
			cflag = 0;
			break;
		case 'b':
			bflag = 1;
			break;
		case 'c':
			cflag = 1;
			aflag = 0;
			break;
		case 'd':
			dflag = 1;
			diff = strtonum(optarg, 0, MAX_HITS, &errstr);
			if (errstr)
				errx(1, "You fool, %s is not a NUMBER we "
					"want! Error: %s", optarg, errstr);
			break;
		case 'D':
			Dflag = 1;
			Iflag = 0;
			break;
		case 'i':
			iflag = 1;
			break;
		case 'I':
			Iflag = 1;
			Dflag = 0;
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
		size_t	 len = 0;
		char	cwd[PATH_MAX];

		memset(&cwd, 0, PATH_MAX);
		if ((getcwd(cwd, sizeof(cwd))) == NULL) {
			err(5, "Error while retrieving the current path");
			goto error;
		}
		len = strlen(cwd) + 2;
		if (len < PATH_MAX)
			snprintf(cwd, len, "%s/", cwd);
		else {
			fprintf(stderr, "path too long\n");
			goto error;
		}

		if (Iflag && !Dflag) {
			if ((rv = db_insert_dir(TABLE_HOME, cwd, dirname))
								!= SQLITE_DONE) {
				if (rv == SQLITE_CONSTRAINT)
					fprintf(stderr, "DB already contains %s%s\n",
						cwd, dirname);
				else
					err(1, "Cannot insert directory %s%s to db",
						cwd, dirname);
				goto error;
			}
		} else if (Dflag) {
			if ((rv = db_delete_dir(TABLE_HOME, cwd, dirname))
								!= SQLITE_DONE) {
				err(1, "Cannot remove directory %s%s from db",
					cwd, dirname);
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

	(void)fprintf(stderr, "usage: %s [-abcdDiIt] directory name\n",
		      __progname);
	exit(1);
}

