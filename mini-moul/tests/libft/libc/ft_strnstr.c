#include <stdio.h>
#include <string.h>
#include "../../../../ft_strnstr.c"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

/* BSD function: expected pointers are hardcoded offsets into the haystack. */
int main(void)
{
	int			error = 0;
	const char	*hay = "hello";

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

	return (error);
}
