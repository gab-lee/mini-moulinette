#include <stdio.h>
#include <ctype.h>
#include "../libft_proto.h"
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
	    {.desc = "ft_isprint(' ') (32, first printable)", .c = ' '},
	    {.desc = "ft_isprint('~') (126, last printable)", .c = '~'},
	    {.desc = "ft_isprint(31) (below range)", .c = 31},
	    {.desc = "ft_isprint(127) (DEL, above range)", .c = 127},
	    {.desc = "ft_isprint('a')", .c = 'a'},
	    {.desc = "ft_isprint('0')", .c = '0'},
	    {.desc = "ft_isprint('\\t')", .c = '\t'},
	    {.desc = "ft_isprint('\\n')", .c = '\n'},
	    {.desc = "ft_isprint('\\0')", .c = '\0'},
	    {.desc = "ft_isprint(200) (extended)", .c = 200},
	    {.desc = "ft_isprint(EOF)", .c = EOF},
	};
	int count = sizeof(tests) / sizeof(tests[0]);
	int error = 0;
	int i;

	for (i = 0; i < count; i++)
		error += check_truthy(i + 1, tests[i].desc,
			ft_isprint(tests[i].c), isprint(tests[i].c));
	error += sweep_truthy(count + 1, "ft_isprint", ft_isprint, isprint);

	return (error);
}
