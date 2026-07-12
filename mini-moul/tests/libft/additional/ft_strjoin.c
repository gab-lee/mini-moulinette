#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static int	strjoin_case(int i, char *desc, char const *s1, char const *s2,
			char *expected)
{
	char	*res;

	res = ft_strjoin(s1, s2);
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
	char	s1[] = "hello, ";
	char	s2[] = "world";
	char	*res;

	error += strjoin_case(1, "ft_strjoin(\"hello, \", \"world\") returns \"hello, world\"",
		s1, s2, "hello, world");
	error += strjoin_case(2, "ft_strjoin(\"\", \"world\") returns \"world\"",
		"", "world", "world");
	error += strjoin_case(3, "ft_strjoin(\"hello\", \"\") returns \"hello\"",
		"hello", "", "hello");
	error += strjoin_case(4, "ft_strjoin(\"\", \"\") returns \"\"",
		"", "", "");

	res = ft_strjoin(s1, s2);
	if (res != NULL)
	{
		if (strcmp(s1, "hello, ") == 0 && strcmp(s2, "world") == 0)
			printf("  " GREEN CHECKMARK GREY " [5] ft_strjoin does not modify its inputs\n" DEFAULT);
		else
		{
			printf("    " RED "[5] ft_strjoin modified one of its inputs\n" DEFAULT);
			error -= 1;
		}
		free(res);
	}
	else
	{
		printf("    " RED "[5] ft_strjoin returned NULL\n" DEFAULT);
		error -= 1;
	}

	return (error);
}
