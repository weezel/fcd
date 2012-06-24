#include "dbutils.h"
#include "dirindexer.h"

#include <stdio.h>


int
main(int argc, const char *argv[])
{
	db_init();
	if (argc < 2)
		walk_directory("/home/weezel/ohj");
	else
		walk_directory(argv[1]);
	//fprintf(stdout, "Directories added: %zu/%zu\n", directories_added, directories_total);

	return 0;
}
