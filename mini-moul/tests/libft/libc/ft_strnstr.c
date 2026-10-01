#include <stdio.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

/* BSD function: expected pointers are hardcoded offsets into the haystack. */
int main(void)
{
	int			error = 0;
	const char	*hay = "hello";
	const char	*big = "aaabcabcd";
	const char	*empty = "";
	const char	*two = "22";

	error += check_ptr(1, "ft_strnstr with empty needle returns haystack", hay,
		ft_strnstr(hay, "", 5), hay);
	error += check_ptr(2, "ft_strnstr finds \"lo\" within len 5", hay,
		ft_strnstr(hay, "lo", 5), hay + 3);
	error += check_ptr(3, "ft_strnstr misses \"lo\" when len 4 cuts it off", hay,
		ft_strnstr(hay, "lo", 4), NULL);
	error += check_ptr(4, "ft_strnstr finds the whole haystack", hay,
		ft_strnstr(hay, "hello", 5), hay);
	error += check_ptr(5, "ft_strnstr returns NULL for an absent needle", hay,
		ft_strnstr(hay, "world", 5), NULL);
	error += check_ptr(6, "ft_strnstr with len far beyond the string still stops at NUL", hay,
		ft_strnstr(hay, "lo", 100), hay + 3);
	error += check_ptr(7, "ft_strnstr with len 0 finds nothing", hay,
		ft_strnstr(hay, "h", 0), NULL);
	error += check_ptr(8, "ft_strnstr(\"aaabcabcd\", \"aabc\", 0) returns NULL", big,
		ft_strnstr(big, "aabc", 0), NULL);
	error += check_ptr(9, "ft_strnstr(\"aaabcabcd\", \"aabc\", -1) retries after a partial match", big,
		ft_strnstr(big, "aabc", (size_t)-1), big + 1);
	error += check_ptr(10, "ft_strnstr(\"aaabcabcd\", \"a\", -1)", big,
		ft_strnstr(big, "a", (size_t)-1), big);
	error += check_ptr(11, "ft_strnstr(\"aaabcabcd\", \"c\", -1)", big,
		ft_strnstr(big, "c", (size_t)-1), big + 4);
	error += check_ptr(12, "ft_strnstr(\"\", \"\", -1) returns haystack", empty,
		ft_strnstr(empty, "", (size_t)-1), empty);
	error += check_ptr(13, "ft_strnstr(\"\", \"\", 0) returns haystack", empty,
		ft_strnstr(empty, "", 0), empty);
	error += check_ptr(14, "ft_strnstr(\"\", \"coucou\", -1) returns NULL", empty,
		ft_strnstr(empty, "coucou", (size_t)-1), NULL);
	error += check_ptr(15, "ft_strnstr(\"aaabcabcd\", \"aaabc\", 5) matches ending exactly at len", big,
		ft_strnstr(big, "aaabc", 5), big);
	error += check_ptr(16, "ft_strnstr(\"\", \"12345\", 5) returns NULL", empty,
		ft_strnstr(empty, "12345", 5), NULL);
	error += check_ptr(17, "ft_strnstr(\"aaabcabcd\", \"abcd\", 9)", big,
		ft_strnstr(big, "abcd", 9), big + 5);
	error += check_ptr(18, "ft_strnstr(\"aaabcabcd\", \"cd\", 8) returns NULL (match ends past len)", big,
		ft_strnstr(big, "cd", 8), NULL);
	error += check_ptr(19, "ft_strnstr(\"aaabcabcd\", \"a\", 1)", big,
		ft_strnstr(big, "a", 1), big);
	error += check_ptr(20, "ft_strnstr(\"1\", \"a\", 1) returns NULL", "1",
		ft_strnstr("1", "a", 1), NULL);
	error += check_ptr(21, "ft_strnstr(\"22\", \"b\", 2) returns NULL", "22",
		ft_strnstr("22", "b", 2), NULL);
	error += check_ptr(22, "ft_strnstr(\"22\", \"22\", 3) with len past the NUL", two,
		ft_strnstr(two, "22", 3), two);
	error += check_ptr(23, "ft_strnstr(\"aaabcabcd\", \"\", 0) returns haystack", big,
		ft_strnstr(big, "", 0), big);

	return (error);
}
