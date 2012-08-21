/* This approach (ftw) has certain drawbacks, one, the most important is to
 * understand that indexable directory paths should be given in
 * the following format:
 * 	/home/user
 * but not like this:
 * 	../user
 */

#include <sys/types.h>
#include <sys/dirent.h>
#include <sys/param.h>

#include <err.h>
#include <fcntl.h>
#include <ftw.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "extern.h"
#include "dbutils.h"

struct diritem *alloc_diritem(const char *);
void	free_diritem(struct diritem *);
int	walk_directory(const char *);
int	isdirapplicable(const char *);
int	dirlist(const char *, const struct stat *, int);


struct diritem *
alloc_diritem(const char *path)
{
	size_t		 i = 0;
	char		*ps;
	char		*pre;
	struct diritem	*di;

	ps = strrchr(path, '/');
	ps++;
	pre = path;
	if (pre == NULL) {
		warnx("Problem with path %s", path);
		return NULL;
	}

	if ((di = malloc(sizeof(struct diritem))) == NULL)
		err(1, "Diritem memory allocation failed");
	memset(di, 0, sizeof(struct diritem));

	/* At first, copy directory path */
	for (i = 0; pre != ps; pre++, i++) {
		di->path[i] = *pre;
	}
	if (i < MAXPATHLEN)
		di->path[i] = '\0';
	else
		di->path[MAXPATHLEN - 1] = '\0';

	/* Secondly, copy directory name */
	i = strlcpy(di->dname, ps, MAXPATHLEN - 1);
	if (i < MAXNAMLEN)
		di->dname[i] = '\0';
	else
		di->dname[MAXNAMLEN - 1] = '\0';

	return di;
}

void
free_diritem(struct diritem *di)
{
	if (di)
		free(di);
	di = NULL;
}

int
walk_directory(const char *dirname)
{
	int		exists = 0;
	int		ftw_success = -1;
	struct stat	sb;

	exists = !stat(dirname, &sb);
	if (exists && S_ISDIR(sb.st_mode))
		ftw_success = ftw(dirname, dirlist, 1);
	return ftw_success;
}

int
isdirapplicable(const char *path)
{
	char	*p;

	p = strchr(path, '/');
	while (p != NULL) {
		p++;
		if (*p == '.')
			return 1;
		p = strchr(p, '/');
	}

	//if (access(path, R_OK) == 0)
	//	return 0;

	return 0;
}

int
dirlist(const char *name, const struct stat *st, int type)
{
	int	fstatus = 0;

	switch (type) {
	case FTW_NS: /* Symbolic link or another non stat'able object */
		fstatus = -1;
		break;
	case FTW_D:
		if (isdirapplicable(name) == 0) {
			struct diritem *di;

			if ((di = alloc_diritem(name)) == NULL)
				err(1, "Failed to alloc memory for %s", name);

			fprintf(stdout, "[ %s ]%1s %-30s %s\n", "OK", "", di->dname, di->path);
			db_insert_diritem(di);
			free_diritem(di);
			directories_added++;
		}
		directories_total++;
		break;
	default:
		break;
	}

	return 0;
}

