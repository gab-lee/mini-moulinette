#include <stdio.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

/*
** BSD function: hardcoded expectations (see ft_strlcpy.c).
** Rule: returns min(size, strlen(initial dst)) + strlen(src).
*/
static int	lcat_case(int i, char *desc, char *init, char *src, size_t size,
			long exp_ret, char *exp_dst)
{
	char	dst[32];
	long	ret;

	memset(dst, 0, 32);
	strcpy(dst, init);
	ret = (long)ft_strlcat(dst, src, size);
	if (ret == exp_ret && strcmp(dst, exp_dst) == 0)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
		return (0);
	}
	printf("    " RED "[%d] %s: expected ret %ld dst \"%s\", got ret %ld dst \"%.31s\"\n" DEFAULT,
		i, desc, exp_ret, exp_dst, ret, dst);
	return (-1);
}

int main(void)
{
	int error = 0;

	error += lcat_case(1, "ft_strlcat appends fully when size allows",
		"Hello, ", "world", 32, 12, "Hello, world");
	error += lcat_case(2, "ft_strlcat truncates at size 10, returns 12",
		"Hello, ", "world", 10, 12, "Hello, wo");
	error += lcat_case(3, "ft_strlcat with size 3 < dst length appends nothing, returns 8",
		"Hello, ", "world", 3, 8, "Hello, ");
	error += lcat_case(4, "ft_strlcat with size 0 returns strlen(src), dst untouched",
		"Hello, ", "world", 0, 5, "Hello, ");
	error += lcat_case(5, "ft_strlcat with empty src returns dst length",
		"Hello, ", "", 32, 7, "Hello, ");
	error += lcat_case(6, "ft_strlcat onto empty dst behaves like strlcpy",
		"", "abc", 32, 3, "abc");

	return (error);
}
