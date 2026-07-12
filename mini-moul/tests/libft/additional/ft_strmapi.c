#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static char	upper_odd_index(unsigned int i, char c)
{
	if (i % 2 == 1)
		return (toupper((unsigned char)c));
	return (c);
}

static char	replace_with_index(unsigned int i, char c)
{
	(void)c;
	return ('0' + (i % 10));
}

static int	strmapi_case(int i, char *desc, char const *s,
			char (*f)(unsigned int, char), char *expected)
{
	char	*res;

	res = ft_strmapi(s, f);
	if (res == NULL)
	{
		printf("    " RED "[%d] %s: returned NULL\n" DEFAULT, i, desc);
		return (-1);
	}
	if (strcmp(res, expected) == 0)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
		free(res);
		return (0);
	}
	printf("    " RED "[%d] %s: expected \"%s\", got \"%s\"\n" DEFAULT,
		i, desc, expected, res);
	free(res);
	return (-1);
}

int main(void)
{
	int		error = 0;
	char	orig[] = "hello";
	char	*res;

	error += strmapi_case(1, "ft_strmapi(\"hello\", upper_odd_index) uppercases odd indexes",
		"hello", upper_odd_index, "hElLo");
	error += strmapi_case(2, "ft_strmapi(\"\", upper_odd_index) on empty string returns \"\"",
		"", upper_odd_index, "");
	error += strmapi_case(3, "ft_strmapi(\"abcdefghijk\", replace_with_index) uses the index",
		"abcdefghijk", replace_with_index, "01234567890");

	res = ft_strmapi(orig, upper_odd_index);
	if (res != NULL)
	{
		if (strcmp(orig, "hello") == 0)
			printf("  " GREEN CHECKMARK GREY " [4] ft_strmapi does not modify the original string\n" DEFAULT);
		else
		{
			printf("    " RED "[4] ft_strmapi modified the original string\n" DEFAULT);
			error -= 1;
		}
		free(res);
	}
	else
	{
		printf("    " RED "[4] ft_strmapi returned NULL\n" DEFAULT);
		error -= 1;
	}

	return (error);
}
