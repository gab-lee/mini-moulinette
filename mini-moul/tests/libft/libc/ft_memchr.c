#include <stdio.h>
#include <string.h>
#include "../../../../ft_memchr.c"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

int main(void)
{
	int				error = 0;
	const char		*s = "hello world";
	unsigned char	ext[4] = {1, 200, 3, 0};

	error += check_ptr(1, "ft_memchr finds 'w' within n=11", s,
		ft_memchr(s, 'w', 11), memchr(s, 'w', 11));
	error += check_ptr(2, "ft_memchr returns NULL when 'w' is beyond n=5", s,
		ft_memchr(s, 'w', 5), memchr(s, 'w', 5));
	error += check_ptr(3, "ft_memchr finds the first byte with n=1", s,
		ft_memchr(s, 'h', 1), memchr(s, 'h', 1));
	error += check_ptr(4, "ft_memchr finds '\\0' when n covers it", s,
		ft_memchr(s, '\0', 12), memchr(s, '\0', 12));
	error += check_ptr(5, "ft_memchr with n=0 returns NULL", s,
		ft_memchr(s, 'h', 0), memchr(s, 'h', 0));
	error += check_ptr(6, "ft_memchr finds byte 200 (unsigned compare)", ext,
		ft_memchr(ext, 200, 4), memchr(ext, 200, 4));

	return (error);
}
