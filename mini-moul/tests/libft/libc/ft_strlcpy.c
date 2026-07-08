#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "../../../../ft_strlcpy.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char *desc;
	char *expected;
} t_test;

int run_tests(t_test *tests, int count);

int main(void)
{
	t_test tests[] = {
	    {.desc = "TODO: ft_strlcpy test cases not written yet",
	     .expected = ""},
	    // Add test cases here
	};
	int count = sizeof(tests) / sizeof(tests[0]);

	return (run_tests(tests, count));
}

int run_tests(t_test *tests, int count)
{
	int i;
	int error = 0;

	for (i = 0; i < count; i++)
	{
		// TODO: call ft_strlcpy and compare the result against tests[i].expected
		printf("    " RED "[%d] %s\n" DEFAULT, i + 1, tests[i].desc);
		error -= 1;
	}

	return (error);
}
