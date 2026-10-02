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
	error += lcat_case(7, "ft_strlcat(\"B\", \"AAAAAAAAA\", 0) returns 9, dst untouched",
		"B", "AAAAAAAAA", 0, 9, "B");
	error += lcat_case(8, "ft_strlcat(\"B\", \"AAAAAAAAA\", 1) returns 10, dst untouched",
		"B", "AAAAAAAAA", 1, 10, "B");
	error += lcat_case(9, "ft_strlcat(\"BBBB\", \"AAAAAAAAA\", 3) returns 12, dst untouched",
		"BBBB", "AAAAAAAAA", 3, 12, "BBBB");
	error += lcat_case(10, "ft_strlcat(\"BBBB\", \"AAAAAAAAA\", 6) returns 13, appends one char",
		"BBBB", "AAAAAAAAA", 6, 13, "BBBBA");
	error += lcat_case(11, "ft_strlcat(\"CCCCC\", \"AAAAAAAAA\", -1 (SIZE_MAX)) appends fully, returns 14",
		"CCCCC", "AAAAAAAAA", (size_t)-1, 14, "CCCCCAAAAAAAAA");
	error += lcat_case(12, "ft_strlcat(15 x 'C', \"AAAAAAAAA\", 17) returns 24, appends one char",
		"CCCCCCCCCCCCCCC", "AAAAAAAAA", 17, 24, "CCCCCCCCCCCCCCCA");
	error += lcat_case(13, "ft_strlcat(\"\", \"AAAAAAAAA\", 1) returns 9, dst stays \"\"",
		"", "AAAAAAAAA", 1, 9, "");
	error += lcat_case(14, "ft_strlcat(\"1111111111\", \"AAAAAAAAA\", 5) returns 14, dst untouched",
		"1111111111", "AAAAAAAAA", 5, 14, "1111111111");
	error += lcat_case(15, "ft_strlcat(\"1111111111\", \"\", 15) returns 10",
		"1111111111", "", 15, 10, "1111111111");
	error += lcat_case(16, "ft_strlcat(\"\", \"\", 42) returns 0",
		"", "", 42, 0, "");
	error += lcat_case(17, "ft_strlcat(\"\", \"\", 0) returns 0",
		"", "", 0, 0, "");
	error += lcat_case(18, "ft_strlcat(\"\", \"123\", 0) returns 3, dst stays \"\"",
		"", "123", 0, 3, "");
	error += lcat_case(19, "ft_strlcat(\"\", \"123\", 1) returns 3, dst stays \"\"",
		"", "123", 1, 3, "");
	error += lcat_case(20, "ft_strlcat(\"\", \"123\", 2) returns 3, dst \"1\"",
		"", "123", 2, 3, "1");
	error += lcat_case(21, "ft_strlcat(\"\", \"123\", 3) returns 3, dst \"12\"",
		"", "123", 3, 3, "12");
	error += lcat_case(22, "ft_strlcat(\"\", \"123\", 4) returns 3, dst \"123\"",
		"", "123", 4, 3, "123");

	return (error);
}
