#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static int	substr_case(int i, char *desc, char const *s, unsigned int start,
			size_t len, char *expected)
{
	char	*res;

	res = ft_substr(s, start, len);
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
	char	*res;

	error += substr_case(1, "ft_substr(\"hello, world\", 0, 5) returns \"hello\"",
		"hello, world", 0, 5, "hello");
	error += substr_case(2, "ft_substr(\"hello, world\", 7, 5) returns \"world\"",
		"hello, world", 7, 5, "world");
	error += substr_case(3, "ft_substr(\"hello\", 0, 100) clamps len to the string end",
		"hello", 0, 100, "hello");
	error += substr_case(4, "ft_substr(\"hello\", 5, 3) start at NUL returns \"\"",
		"hello", 5, 3, "");
	error += substr_case(5, "ft_substr(\"hello\", 10, 3) start past the end returns \"\"",
		"hello", 10, 3, "");
	error += substr_case(6, "ft_substr(\"hello\", 0, 0) returns \"\"",
		"hello", 0, 0, "");
	error += substr_case(7, "ft_substr(\"\", 0, 5) on empty string returns \"\"",
		"", 0, 5, "");
	error += substr_case(8, "ft_substr(\"hello\", 1, 3) returns \"ell\"",
		"hello", 1, 3, "ell");

	res = ft_substr("hello", 0, 3);
	if (res != NULL)
	{
		printf("  " GREEN CHECKMARK GREY " [9] ft_substr allocates a new buffer\n" DEFAULT);
		free(res);
	}
	else
	{
		printf("    " RED "[9] ft_substr(\"hello\", 0, 3) returned NULL\n" DEFAULT);
		error -= 1;
	}

	return (error);
}
