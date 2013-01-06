#ifndef _EXTERN_H_
#define _EXTERN_H_

#if defined __OpenBSD__
#include <sys/dirent.h>
#else
#if !defined(MAXNAMLEN)
#define MAXNAMLEN NAME_MAX
#endif
#endif

#include <sys/param.h>

#define MAX_HITS 256
#define MAX_CHOICESIZE (MAXPATHLEN + MAXNAMLEN)

#define DEBUG

#define TABLE_HOME "homedir"
#define TABLE_USR  "usrdir"
#define TABLE_ETC  "etcdir"

enum table_column { BOOKMARK, VISIT } tc;

struct diritem {
	char	path[MAXPATHLEN];
	char	dname[MAXNAMLEN];
};

long int	directories_added;
long int	directories_total;

#endif
