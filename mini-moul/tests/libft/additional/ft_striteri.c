#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static void	upper_even_index(unsigned int i, char *c)
{
	if (i % 2 == 0)
		*c = toupper((unsigned char)*c);
}

static void	replace_with_index(unsigned int i, char *c)
{
	*c = '0' + (i % 10);
}

static int	striteri_case(int i, char *desc, char *s,
			void (*f)(unsigned int, char *), char *expected)
{
	ft_striteri(s, f);
	if (strcmp(s, expected) == 0)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
		return (0);
	}
	printf("    " RED "[%d] %s: expected \"%s\", got \"%s\"\n" DEFAULT,
		i, desc, expected, s);
	return (-1);
}

int main(void)
{
	int		error = 0;
	char	s1[] = "hello";
	char	s2[] = "";
	char	s3[] = "abcdefghijk";

	error += striteri_case(1, "ft_striteri(\"hello\", upper_even_index) uppercases in place",
		s1, upper_even_index, "HeLlO");
	error += striteri_case(2, "ft_striteri(\"\", ...) on empty string leaves it untouched",
		s2, upper_even_index, "");
	error += striteri_case(3, "ft_striteri(\"abcdefghijk\", replace_with_index) uses the index",
		s3, replace_with_index, "01234567890");

	return (error);
}
