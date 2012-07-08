#include "dbutils.h"
#include "extern.h"

#include <stdio.h>

void test_walk_directory(void);
void test_db_find_exact_match(void);
void test_db_get_choice_by_id(void);
int walk_directory(const char *);


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

void
test_db_get_choice_by_id(void)
{
	struct resultset	rs;

	rs = db_get_choice_by_id(TABLE_HOME, 1);
	fprintf(stdout, "[%lu] %s %s %lu %d\n", rs.id, rs.path, rs.dir, rs.visits, rs.bookmark);
}

int
main(void)
{
	db_init();

	//test_walk_directory();
	//test_db_find_exact_match();
	test_db_get_choice_by_id();

	return 0;
}
