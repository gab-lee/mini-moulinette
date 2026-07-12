#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static int	split_case(int i, char *desc, char const *s, char c,
			char **expected, int expected_count)
{
	char	**res;
	int		k;

	res = ft_split(s, c);
	if (res == NULL)
	{
		printf("    " RED "[%d] %s: returned NULL\n" DEFAULT, i, desc);
		return (-1);
	}
	k = 0;
	while (k < expected_count)
	{
		if (res[k] == NULL || strcmp(res[k], expected[k]) != 0)
		{
			printf("    " RED "[%d] %s: element %d expected \"%s\", got \"%s\"\n" DEFAULT,
				i, desc, k, expected[k], res[k] ? res[k] : "(null)");
			free_split(res);
			return (-1);
		}
		k++;
	}
	if (res[expected_count] != NULL)
	{
		printf("    " RED "[%d] %s: array has extra elements past index %d, or is not NULL-terminated\n" DEFAULT,
			i, desc, expected_count - 1);
		free_split(res);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
	free_split(res);
	return (0);
}

int main(void)
{
	int		error = 0;
	char	*words[] = {"hello", "world"};
	char	**none = NULL;
	char	*single[] = {"hello"};
	char	*three[] = {"a", "b", "c"};
	char	*ab[] = {"a", "b"};

	error += split_case(1, "ft_split(\"hello world\", ' ') splits on a single space",
		"hello world", ' ', words, 2);
	error += split_case(2, "ft_split(\"  hello   world  \", ' ') collapses repeated separators",
		"  hello   world  ", ' ', words, 2);
	error += split_case(3, "ft_split(\"\", ' ') on empty string returns an empty array",
		"", ' ', none, 0);
	error += split_case(4, "ft_split(\"   \", ' ') on separators-only returns an empty array",
		"   ", ' ', none, 0);
	error += split_case(5, "ft_split(\"hello\", ' ') with no separator returns one element",
		"hello", ' ', single, 1);
	error += split_case(6, "ft_split(\"a,b,c\", ',') splits on comma",
		"a,b,c", ',', three, 3);
	error += split_case(7, "ft_split(\",a,,b,\", ',') ignores leading/trailing/doubled separators",
		",a,,b,", ',', ab, 2);

	return (error);
}
