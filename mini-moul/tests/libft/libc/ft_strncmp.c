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
	error += check_sign(10, "ft_strncmp(\"t\", \"\", 0)",
		ft_strncmp("t", "", 0), strncmp("t", "", 0));
	error += check_sign(11, "ft_strncmp(\"1234\", \"1235\", 3)",
		ft_strncmp("1234", "1235", 3), strncmp("1234", "1235", 3));
	error += check_sign(12, "ft_strncmp(\"1234\", \"1235\", 4)",
		ft_strncmp("1234", "1235", 4), strncmp("1234", "1235", 4));
	error += check_sign(13, "ft_strncmp(\"1234\", \"1235\", -1 (SIZE_MAX))",
		ft_strncmp("1234", "1235", (size_t)-1), strncmp("1234", "1235", (size_t)-1));
	error += check_sign(14, "ft_strncmp(\"\", \"\", 42)",
		ft_strncmp("", "", 42), strncmp("", "", 42));
	error += check_sign(15, "ft_strncmp(\"Tripouille\", \"Tripouille\", 42)",
		ft_strncmp("Tripouille", "Tripouille", 42), strncmp("Tripouille", "Tripouille", 42));
	error += check_sign(16, "ft_strncmp(\"Tripouille\", \"tripouille\", 42)",
		ft_strncmp("Tripouille", "tripouille", 42), strncmp("Tripouille", "tripouille", 42));
	error += check_sign(17, "ft_strncmp(\"Tripouille\", \"TriPouille\", 42)",
		ft_strncmp("Tripouille", "TriPouille", 42), strncmp("Tripouille", "TriPouille", 42));
	error += check_sign(18, "ft_strncmp(\"Tripouille\", \"TripouillE\", 42)",
		ft_strncmp("Tripouille", "TripouillE", 42), strncmp("Tripouille", "TripouillE", 42));
	error += check_sign(19, "ft_strncmp(\"Tripouille\", \"TripouilleX\", 42)",
		ft_strncmp("Tripouille", "TripouilleX", 42), strncmp("Tripouille", "TripouilleX", 42));
	error += check_sign(20, "ft_strncmp(\"Tripouille\", \"Tripouill\", 42)",
		ft_strncmp("Tripouille", "Tripouill", 42), strncmp("Tripouille", "Tripouill", 42));
	error += check_sign(21, "ft_strncmp(\"\", \"1\", 0)",
		ft_strncmp("", "1", 0), strncmp("", "1", 0));
	error += check_sign(22, "ft_strncmp(\"1\", \"\", 0)",
		ft_strncmp("1", "", 0), strncmp("1", "", 0));
	error += check_sign(23, "ft_strncmp(\"\", \"1\", 1)",
		ft_strncmp("", "1", 1), strncmp("", "1", 1));
	error += check_sign(24, "ft_strncmp(\"1\", \"\", 1)",
		ft_strncmp("1", "", 1), strncmp("1", "", 1));
	error += check_sign(25, "ft_strncmp(\"\", \"\", 1)",
		ft_strncmp("", "", 1), strncmp("", "", 1));
	error += check_sign(26, "ft_strncmp(\"test\", \"tes\\x80\", 4) (SCHAR_MIN byte)",
		ft_strncmp("test", "tes\x80", 4), strncmp("test", "tes\x80", 4));
	error += check_sign(27, "ft_strncmp(\"test\", \"tes\\xd6\", 4) (-42 byte)",
		ft_strncmp("test", "tes\xd6", 4), strncmp("test", "tes\xd6", 4));

	return (error);
}
