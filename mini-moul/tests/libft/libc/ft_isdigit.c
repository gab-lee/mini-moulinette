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
	    {.desc = "ft_isdigit('0')", .c = '0'},
	    {.desc = "ft_isdigit('9')", .c = '9'},
	    {.desc = "ft_isdigit('5')", .c = '5'},
	    {.desc = "ft_isdigit('/') ('0' - 1)", .c = '/'},
	    {.desc = "ft_isdigit(':') ('9' + 1)", .c = ':'},
	    {.desc = "ft_isdigit('a')", .c = 'a'},
	    {.desc = "ft_isdigit('A')", .c = 'A'},
	    {.desc = "ft_isdigit(' ')", .c = ' '},
	    {.desc = "ft_isdigit('\\0')", .c = '\0'},
	    {.desc = "ft_isdigit(200) (extended)", .c = 200},
	    {.desc = "ft_isdigit(EOF)", .c = EOF},
	};
	int count = sizeof(tests) / sizeof(tests[0]);
	int error = 0;
	int i;

	for (i = 0; i < count; i++)
		error += check_truthy(i + 1, tests[i].desc,
			ft_isdigit(tests[i].c), isdigit(tests[i].c));
	error += sweep_truthy(count + 1, "ft_isdigit", ft_isdigit, isdigit);

	return (error);
}
