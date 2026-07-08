#include <stdio.h>
#include <ctype.h>
#include "../../../../ft_isascii.c"
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
	    {.desc = "ft_isascii(0)", .c = 0},
	    {.desc = "ft_isascii(127) (last ascii)", .c = 127},
	    {.desc = "ft_isascii(128) (first non-ascii)", .c = 128},
	    {.desc = "ft_isascii('a')", .c = 'a'},
	    {.desc = "ft_isascii(' ')", .c = ' '},
	    {.desc = "ft_isascii('\\n')", .c = '\n'},
	    {.desc = "ft_isascii(200)", .c = 200},
	    {.desc = "ft_isascii(255)", .c = 255},
	    {.desc = "ft_isascii(EOF)", .c = EOF},
	};
	int count = sizeof(tests) / sizeof(tests[0]);
	int error = 0;
	int i;

	for (i = 0; i < count; i++)
		error += check_truthy(i + 1, tests[i].desc,
			ft_isascii(tests[i].c), isascii(tests[i].c));
	error += sweep_truthy(count + 1, "ft_isascii", ft_isascii, isascii);

	return (error);
}
