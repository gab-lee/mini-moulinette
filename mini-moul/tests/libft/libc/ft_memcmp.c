#include <stdio.h>
#include <string.h>
#include "../../../../ft_memcmp.c"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

int main(void)
{
	int				error = 0;
	unsigned char	hi[2] = {0x80, 0};
	unsigned char	lo[2] = {0x01, 0};

	error += check_sign(1, "ft_memcmp(\"abcdef\", \"abcdef\", 6) (equal)",
		ft_memcmp("abcdef", "abcdef", 6), memcmp("abcdef", "abcdef", 6));
	error += check_sign(2, "ft_memcmp(\"abcdef\", \"abcdeg\", 6) (differs at last)",
		ft_memcmp("abcdef", "abcdeg", 6), memcmp("abcdef", "abcdeg", 6));
	error += check_sign(3, "ft_memcmp(\"abcdeg\", \"abcdef\", 6) (reversed)",
		ft_memcmp("abcdeg", "abcdef", 6), memcmp("abcdeg", "abcdef", 6));
	error += check_sign(4, "ft_memcmp(\"abcdef\", \"abcdeg\", 5) (difference beyond n)",
		ft_memcmp("abcdef", "abcdeg", 5), memcmp("abcdef", "abcdeg", 5));
	error += check_sign(5, "ft_memcmp with n=0 (always equal)",
		ft_memcmp("abc", "xyz", 0), memcmp("abc", "xyz", 0));
	error += check_sign(6, "ft_memcmp({0x80}, {0x01}, 1) (unsigned byte compare)",
		ft_memcmp(hi, lo, 1), memcmp(hi, lo, 1));
	error += check_sign(7, "ft_memcmp(\"a\\0b\", \"a\\0c\", 3) (does NOT stop at NUL)",
		ft_memcmp("a\0b", "a\0c", 3), memcmp("a\0b", "a\0c", 3));

	return (error);
}
