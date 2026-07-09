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
	    {.desc = "ft_isalpha('a')", .c = 'a'},
	    {.desc = "ft_isalpha('z')", .c = 'z'},
	    {.desc = "ft_isalpha('A')", .c = 'A'},
	    {.desc = "ft_isalpha('Z')", .c = 'Z'},
	    {.desc = "ft_isalpha('`') ('a' - 1)", .c = '`'},
	    {.desc = "ft_isalpha('{') ('z' + 1)", .c = '{'},
	    {.desc = "ft_isalpha('@') ('A' - 1)", .c = '@'},
	    {.desc = "ft_isalpha('[') ('Z' + 1)", .c = '['},
	    {.desc = "ft_isalpha('0')", .c = '0'},
	    {.desc = "ft_isalpha('9')", .c = '9'},
	    {.desc = "ft_isalpha(' ')", .c = ' '},
	    {.desc = "ft_isalpha('\\t')", .c = '\t'},
	    {.desc = "ft_isalpha('\\n')", .c = '\n'},
	    {.desc = "ft_isalpha('\\0')", .c = '\0'},
	    {.desc = "ft_isalpha(200) (extended)", .c = 200},
	    {.desc = "ft_isalpha(255) (extended)", .c = 255},
	    {.desc = "ft_isalpha(EOF)", .c = EOF},
	};
	int count = sizeof(tests) / sizeof(tests[0]);
	int error = 0;
	int i;

	for (i = 0; i < count; i++)
		error += check_truthy(i + 1, tests[i].desc,
			ft_isalpha(tests[i].c), isalpha(tests[i].c));
	error += sweep_truthy(count + 1, "ft_isalpha", ft_isalpha, isalpha);

	return (error);
}
