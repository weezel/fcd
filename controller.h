#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <sys/dirent.h>
#include <sys/param.h>

struct dlist {				/* directory list */
	char	abspath[MAXPATHLEN];	/* directory path */
	char	dname[MAXNAMLEN];	/* directory name */
};

struct dlist *dirlist;
int hit_max_items;

void alloc_dlist(size_t);
void free_dlist(void);
void process_query(const char *);
MENU *combine_items(const char *, sqlite3 *, size_t);

#endif
