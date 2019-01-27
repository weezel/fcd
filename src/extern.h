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

int	aflag; /* Ask for the choice, cannot be combined with 'c' flag */
int	bflag; /* Bookmark */
int	cflag; /* Use curses, cannot be combined with 'a' flag */
int	dflag; /* Maximum word difference in spellcheck */
int	Dflag; /* Remove directory item */
int	iflag; /* Case insensitive search */
int	Iflag; /* Insert new directory item */
int	tflag; /* Search for table {ETC, HOME, USR} */
int	vflag; /* Verbose mode */

enum table_column { BOOKMARK, VISIT } tc;

struct diritem {
	char	path[MAXPATHLEN];
	char	dname[MAXNAMLEN];
};

long int	directories_added;
long int	directories_total;

#endif
