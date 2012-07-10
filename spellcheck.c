#include <ctype.h>
#include <err.h>
#include <getopt.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined __linux__
#define u_char unsigned char
#include <bsd/stdlib.h>
#endif

#define LINEBUFFER 1024

int min(const int, const int);
size_t levenstein_distance(const char *, const size_t, const char *, const size_t);


int
min(const int a, const int b)
{
	return a <= b ? a : b;
}

size_t
levenstein_distance(const char *s1, const size_t s1len, const char *s2, const size_t s2len)
{
	size_t	  i, j;
	int	  cost, result, sumofall;
	int	**resultable;

	cost = i = j = sumofall = 0;

	if ((resultable = calloc(s1len + 1, sizeof(int *))) == NULL)
		err(10, "malloc");

	for (i = 0; i < s1len + 1; i++) {
		if ((resultable[i] = calloc(s2len + 1, sizeof(int))) == NULL)
			err(10, "malloc");
	}

	for (i = 0; i <= s1len; i++)
		resultable[i][0] = i;
	for (j = 0; j <= s2len; j++)
		resultable[0][j] = j;

	for (i = 1; i <= s1len; i++) {
		for (j = 1; j <= s2len; j++) {
			cost = (s1[i-1] == s2[j-1]) ?  0 : 1;

			sumofall = min(min(resultable[i-1][j] + 1,		/* deletion */
					   resultable[i][j-1] + 1),		/* insertion */
					   resultable[i-1][j-1] + cost);	/* substitution */

			/*
			if (i > 1 && j > 1) {
				int transpos = resultable[i-1][j-2] + 1;

				if (s1[i-2] != s2[j-1])
					transpos++;
				if (s1[i-1] != s2[j-2])
					transpos++;
				if (sumofall > transpos)
					sumofall = transpos;
			}
			*/
			resultable[i][j] = sumofall;
		}
	}

	result = resultable[s1len][s2len];

	for (i = 0; i < s1len+1; i++)
		free(resultable[i]);
	free(resultable);

	return result;
}

