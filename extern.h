#ifndef _EXTERN_H_
#define _EXTERN_H_

#include <sys/dirent.h>
#include <sys/param.h>

#define MAX_HITS 256
#define MAX_CHOICESIZE (MAXPATHLEN + MAXNAMLEN)

#define TABLE_HOME "homedir"
#define TABLE_USR  "usrdir"
#define TABLE_ETC  "etcdir"

struct diritem {
	char	path[MAXPATHLEN];
	char	dname[MAXNAMLEN];
};

long int	directories_added;
long int	directories_total;

int dflag;	/* Maximum difference between words for spellcheck */

#endif
