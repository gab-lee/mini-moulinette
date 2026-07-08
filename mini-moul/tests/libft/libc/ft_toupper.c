#include <stdio.h>
#include <ctype.h>
#include "../../../../ft_toupper.c"
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
	    {.desc = "ft_toupper('a')", .c = 'a'},
	    {.desc = "ft_toupper('z')", .c = 'z'},
	    {.desc = "ft_toupper('m')", .c = 'm'},
	    {.desc = "ft_toupper('A') (already upper)", .c = 'A'},
	    {.desc = "ft_toupper('Z') (already upper)", .c = 'Z'},
	    {.desc = "ft_toupper('`') ('a' - 1)", .c = '`'},
	    {.desc = "ft_toupper('{') ('z' + 1)", .c = '{'},
	    {.desc = "ft_toupper('0')", .c = '0'},
	    {.desc = "ft_toupper(' ')", .c = ' '},
	    {.desc = "ft_toupper('\\0')", .c = '\0'},
	    {.desc = "ft_toupper(200) (extended)", .c = 200},
	    {.desc = "ft_toupper(EOF)", .c = EOF},
	};
	int count = sizeof(tests) / sizeof(tests[0]);
	int error = 0;
	int i;

	for (i = 0; i < count; i++)
		error += check_exact(i + 1, tests[i].desc,
			ft_toupper(tests[i].c), toupper(tests[i].c));
	error += sweep_exact(count + 1, "ft_toupper", ft_toupper, toupper);

	return (error);
}
