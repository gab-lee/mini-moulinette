#include <stdio.h>
#include <ctype.h>
#include "../../../../ft_tolower.c"
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
	    {.desc = "ft_tolower('A')", .c = 'A'},
	    {.desc = "ft_tolower('Z')", .c = 'Z'},
	    {.desc = "ft_tolower('M')", .c = 'M'},
	    {.desc = "ft_tolower('a') (already lower)", .c = 'a'},
	    {.desc = "ft_tolower('z') (already lower)", .c = 'z'},
	    {.desc = "ft_tolower('@') ('A' - 1)", .c = '@'},
	    {.desc = "ft_tolower('[') ('Z' + 1)", .c = '['},
	    {.desc = "ft_tolower('0')", .c = '0'},
	    {.desc = "ft_tolower(' ')", .c = ' '},
	    {.desc = "ft_tolower('\\0')", .c = '\0'},
	    {.desc = "ft_tolower(200) (extended)", .c = 200},
	    {.desc = "ft_tolower(EOF)", .c = EOF},
	};
	int count = sizeof(tests) / sizeof(tests[0]);
	int error = 0;
	int i;

	for (i = 0; i < count; i++)
		error += check_exact(i + 1, tests[i].desc,
			ft_tolower(tests[i].c), tolower(tests[i].c));
	error += sweep_exact(count + 1, "ft_tolower", ft_tolower, tolower);

	return (error);
}
