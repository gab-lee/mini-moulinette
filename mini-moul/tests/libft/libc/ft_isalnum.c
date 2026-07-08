#include <stdio.h>
#include <ctype.h>
#include "../../../../ft_isalnum.c"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

typedef struct s_test
{
	char *desc;
	int c;
} t_test;

int main(void)
{
	t_test tests[] = {
	    {.desc = "ft_isalnum('a')", .c = 'a'},
	    {.desc = "ft_isalnum('z')", .c = 'z'},
	    {.desc = "ft_isalnum('A')", .c = 'A'},
	    {.desc = "ft_isalnum('Z')", .c = 'Z'},
	    {.desc = "ft_isalnum('0')", .c = '0'},
	    {.desc = "ft_isalnum('9')", .c = '9'},
	    {.desc = "ft_isalnum('`') ('a' - 1)", .c = '`'},
	    {.desc = "ft_isalnum('{') ('z' + 1)", .c = '{'},
	    {.desc = "ft_isalnum('@') ('A' - 1)", .c = '@'},
	    {.desc = "ft_isalnum('[') ('Z' + 1)", .c = '['},
	    {.desc = "ft_isalnum('/') ('0' - 1)", .c = '/'},
	    {.desc = "ft_isalnum(':') ('9' + 1)", .c = ':'},
	    {.desc = "ft_isalnum(' ')", .c = ' '},
	    {.desc = "ft_isalnum('\\0')", .c = '\0'},
	    {.desc = "ft_isalnum(200) (extended)", .c = 200},
	    {.desc = "ft_isalnum(EOF)", .c = EOF},
	};
	int count = sizeof(tests) / sizeof(tests[0]);
	int error = 0;
	int i;

	for (i = 0; i < count; i++)
		error += check_truthy(i + 1, tests[i].desc,
			ft_isalnum(tests[i].c), isalnum(tests[i].c));
	error += sweep_truthy(count + 1, "ft_isalnum", ft_isalnum, isalnum);

	return (error);
}
