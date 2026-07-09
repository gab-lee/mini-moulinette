#include <stdio.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

int main(void)
{
	int error = 0;

	error += check_sign(1, "ft_strncmp(\"abc\", \"abc\", 3) (equal)",
		ft_strncmp("abc", "abc", 3), strncmp("abc", "abc", 3));
	error += check_sign(2, "ft_strncmp(\"abc\", \"abd\", 3) (differs at 3rd)",
		ft_strncmp("abc", "abd", 3), strncmp("abc", "abd", 3));
	error += check_sign(3, "ft_strncmp(\"abd\", \"abc\", 3) (reversed)",
		ft_strncmp("abd", "abc", 3), strncmp("abd", "abc", 3));
	error += check_sign(4, "ft_strncmp(\"abc\", \"abd\", 2) (difference beyond n)",
		ft_strncmp("abc", "abd", 2), strncmp("abc", "abd", 2));
	error += check_sign(5, "ft_strncmp with n=0 (always equal)",
		ft_strncmp("abc", "xyz", 0), strncmp("abc", "xyz", 0));
	error += check_sign(6, "ft_strncmp(\"test\", \"testing\", 7) (prefix shorter)",
		ft_strncmp("test", "testing", 7), strncmp("test", "testing", 7));
	error += check_sign(7, "ft_strncmp(\"\", \"abc\", 3) (empty vs non-empty)",
		ft_strncmp("", "abc", 3), strncmp("", "abc", 3));
	error += check_sign(8, "ft_strncmp(\"\\xff\", \"\\x01\", 1) (unsigned char compare)",
		ft_strncmp("\xff", "\x01", 1), strncmp("\xff", "\x01", 1));
	error += check_sign(9, "ft_strncmp(\"a\\0x\", \"a\\0y\", 3) (stops at NUL)",
		ft_strncmp("a\0x", "a\0y", 3), strncmp("a\0x", "a\0y", 3));

	return (error);
}
