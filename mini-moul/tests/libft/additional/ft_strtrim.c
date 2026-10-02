#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/alloc_check.h"

static int	strtrim_case(int i, char *desc, char const *s1, char const *set,
			char *expected)
{
	char	*res;
	int		ret;

	res = ft_strtrim(s1, set);
	if (res == NULL)
	{
		printf("    " RED "[%d] %s: returned NULL\n" DEFAULT, i, desc);
		return (-1);
	}
	if (strcmp(res, expected) == 0)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
		ret = check_alloc_size(i, desc, res, strlen(expected) + 1);
		free(res);
		return (ret);
	}
	printf("    " RED "[%d] %s: expected \"%s\", got \"%s\"\n" DEFAULT,
		i, desc, expected, res);
	free(res);
	return (-1);
}

int main(void)
{
	int	error = 0;

	error += strtrim_case(1, "ft_strtrim(\"  hello  \", \" \") trims both sides",
		"  hello  ", " ", "hello");
	error += strtrim_case(2, "ft_strtrim(\"xxhelloxx\", \"x\") trims a custom set",
		"xxhelloxx", "x", "hello");
	error += strtrim_case(3, "ft_strtrim(\"hello\", \"xyz\") with no matching chars returns a copy",
		"hello", "xyz", "hello");
	error += strtrim_case(4, "ft_strtrim(\"xxxx\", \"x\") trims everything, returns \"\"",
		"xxxx", "x", "");
	error += strtrim_case(5, "ft_strtrim(\"\", \"x\") on empty string returns \"\"",
		"", "x", "");
	error += strtrim_case(6, "ft_strtrim(\"hello\", \"\") with empty set returns a copy",
		"hello", "", "hello");
	error += strtrim_case(7, "ft_strtrim(\"xhelloy\", \"xy\") trims only leading/trailing set chars",
		"xhelloy", "xy", "hello");
	error += strtrim_case(8, "ft_strtrim(\"hxelloh\", \"h\") leaves inner chars alone",
		"hxelloh", "h", "xello");
	error += strtrim_case(9, "ft_strtrim(\"   xxxtripouille\", \" x\") trims the start",
		"   xxxtripouille", " x", "tripouille");
	error += strtrim_case(10, "ft_strtrim(\"tripouille   xxx\", \" x\") trims the end",
		"tripouille   xxx", " x", "tripouille");
	error += strtrim_case(11, "ft_strtrim(\"   xxxtripouille   xxx\", \" x\") trims both sides",
		"   xxxtripouille   xxx", " x", "tripouille");
	error += strtrim_case(12, "ft_strtrim(\"   xxx   xxx\", \" x\") trims everything",
		"   xxx   xxx", " x", "");
	error += strtrim_case(13, "ft_strtrim(\"\", \"123\") returns \"\"",
		"", "123", "");
	error += strtrim_case(14, "ft_strtrim(\"123\", \"\") returns a copy",
		"123", "", "123");
	error += strtrim_case(15, "ft_strtrim(\"\", \"\") returns \"\"",
		"", "", "");
	error += strtrim_case(16, "ft_strtrim(\"abcdba\", \"acb\") returns \"d\"",
		"abcdba", "acb", "d");
	error += strtrim_case(17, "ft_strtrim(\"ababa\", \"a\") returns \"bab\"",
		"ababa", "a", "bab");

	return (error);
}
