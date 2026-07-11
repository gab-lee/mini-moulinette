#include <stdio.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

int main(void)
{
	int			error = 0;
	const char	*s = "hello, world";
	char		ext[6] = {'a', 'b', (char)233, 'c', 'd', '\0'};

	error += check_ptr(1, "ft_strchr finds the first 'l'", s,
		ft_strchr(s, 'l'), strchr(s, 'l'));
	error += check_ptr(2, "ft_strchr finds the first char 'h'", s,
		ft_strchr(s, 'h'), strchr(s, 'h'));
	error += check_ptr(3, "ft_strchr finds 'o' (also present later)", s,
		ft_strchr(s, 'o'), strchr(s, 'o'));
	error += check_ptr(4, "ft_strchr returns NULL when absent ('z')", s,
		ft_strchr(s, 'z'), strchr(s, 'z'));
	error += check_ptr(5, "ft_strchr('\\0') returns the terminator", s,
		ft_strchr(s, '\0'), strchr(s, '\0'));
	error += check_ptr(6, "ft_strchr on empty string for '\\0'", "",
		ft_strchr("", '\0'), strchr("", '\0'));
	error += warn_ptr(7, "ft_strchr finds extended char 233", ext,
		ft_strchr(ext, 233), strchr(ext, 233));

	return (error);
}
