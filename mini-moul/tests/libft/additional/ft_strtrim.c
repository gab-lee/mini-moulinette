#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static int	strtrim_case(int i, char *desc, char const *s1, char const *set,
			char *expected)
{
	char	*res;

	res = ft_strtrim(s1, set);
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

	return (error);
}
