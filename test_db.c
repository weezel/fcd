#include "dbutils.h"
#include "extern.h"

#include <stdio.h>


void
test_walk_directory(void)
{
	walk_directory("/home/weezel/ohj");
}

void
test_db_find_exact_match(void)
{
	db_find_exact(TABLE_HOME, "ui");
}

int
main(int argc, const char *argv[])
{
	db_init();

	//test_walk_directory();
	test_db_find_exact_match();

	return 0;
}
