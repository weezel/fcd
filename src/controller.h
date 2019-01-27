#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#if defined __OpenBSD__
#include <sys/dirent.h>
#endif
#if defined __linux__
#include <limits.h>
#include <dirent.h>
#include <bsd/string.h>
#endif
#include <sys/param.h>

#include <menu.h>

struct dlist {				/* directory list */
	char	abspath[MAXPATHLEN];	/* directory path */
	char	dname[MAXNAMLEN];	/* directory name */
};

int		 dbisopen;
int		 hit_max_items;
struct dlist	*dirlist;

void alloc_dlist(size_t);
void free_dlist(void);
void process_query(const char *, const int);
MENU *combine_items(const char *, size_t);

#endif
