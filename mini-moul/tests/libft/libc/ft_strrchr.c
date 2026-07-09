#include <stdio.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

int main(void)
{
	int			error = 0;
	const char	*s = "hello, world";
	char		ext[7] = {'a', (char)233, 'b', (char)233, 'c', 'd', '\0'};

	error += check_ptr(1, "ft_strrchr finds the last 'l'", s,
		ft_strrchr(s, 'l'), strrchr(s, 'l'));
	error += check_ptr(2, "ft_strrchr finds the last 'o'", s,
		ft_strrchr(s, 'o'), strrchr(s, 'o'));
	error += check_ptr(3, "ft_strrchr finds single occurrence 'h'", s,
		ft_strrchr(s, 'h'), strrchr(s, 'h'));
	error += check_ptr(4, "ft_strrchr returns NULL when absent ('z')", s,
		ft_strrchr(s, 'z'), strrchr(s, 'z'));
	error += check_ptr(5, "ft_strrchr('\\0') returns the terminator", s,
		ft_strrchr(s, '\0'), strrchr(s, '\0'));
	error += check_ptr(6, "ft_strrchr on empty string for '\\0'", "",
		ft_strrchr("", '\0'), strrchr("", '\0'));
	error += check_ptr(7, "ft_strrchr finds the last extended char 233", ext,
		ft_strrchr(ext, 233), strrchr(ext, 233));

	return (error);
}
