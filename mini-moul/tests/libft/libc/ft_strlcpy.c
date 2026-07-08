#include <stdio.h>
#include <string.h>
#include "../../../../ft_strlcpy.c"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

/*
** strlcpy/strlcat are BSD functions not present in every libc, so these
** tests use hardcoded expected values instead of comparing against libc.
** exp_dst == NULL means dst must be left completely untouched (size 0).
*/
static int	lcpy_case(int i, char *desc, char *src, size_t size,
			long exp_ret, char *exp_dst)
{
	char	dst[32];
	long	ret;

	memset(dst, 'X', 31);
	dst[31] = '\0';
	ret = (long)ft_strlcpy(dst, src, size);
	if (ret == exp_ret
		&& ((exp_dst == NULL && dst[0] == 'X')
			|| (exp_dst != NULL && strcmp(dst, exp_dst) == 0)))
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
		return (0);
	}
	printf("    " RED "[%d] %s: expected ret %ld dst \"%s\", got ret %ld dst \"%.31s\"\n" DEFAULT,
		i, desc, exp_ret, exp_dst ? exp_dst : "(untouched)", ret, dst);
	return (-1);
}

int main(void)
{
	int error = 0;

	error += lcpy_case(1, "ft_strlcpy(\"hello\", size 32) copies fully, returns 5",
		"hello", 32, 5, "hello");
	error += lcpy_case(2, "ft_strlcpy(\"hello\", size 3) truncates to \"he\", still returns 5",
		"hello", 3, 5, "he");
	error += lcpy_case(3, "ft_strlcpy(\"hello\", size 1) writes only the NUL",
		"hello", 1, 5, "");
	error += lcpy_case(4, "ft_strlcpy(\"hello\", size 0) touches nothing, returns 5",
		"hello", 0, 5, NULL);
	error += lcpy_case(5, "ft_strlcpy(\"\", size 32) returns 0",
		"", 32, 0, "");

	return (error);
}
