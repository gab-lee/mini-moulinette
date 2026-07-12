#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static int	itoa_case(int i, char *desc, int n, char *expected)
{
	char	*res;

	res = ft_itoa(n);
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

	error += itoa_case(1, "ft_itoa(0) returns \"0\"", 0, "0");
	error += itoa_case(2, "ft_itoa(42) returns \"42\"", 42, "42");
	error += itoa_case(3, "ft_itoa(-42) returns \"-42\"", -42, "-42");
	error += itoa_case(4, "ft_itoa(1) returns \"1\"", 1, "1");
	error += itoa_case(5, "ft_itoa(-1) returns \"-1\"", -1, "-1");
	error += itoa_case(6, "ft_itoa(INT_MAX) returns \"2147483647\"",
		INT_MAX, "2147483647");
	error += itoa_case(7, "ft_itoa(INT_MIN) returns \"-2147483648\"",
		INT_MIN, "-2147483648");
	error += itoa_case(8, "ft_itoa(100) returns \"100\"", 100, "100");
	error += itoa_case(9, "ft_itoa(-100) returns \"-100\"", -100, "-100");

	return (error);
}
