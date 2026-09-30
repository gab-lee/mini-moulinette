#include <stdio.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

static int	memcpy_case(int i, char *desc, const void *src, size_t n)
{
	unsigned char	a[64];
	unsigned char	b[64];
	int				error = 0;

	memset(a, 0xAA, 64);
	memset(b, 0xAA, 64);
	if (ft_memcpy(a, src, n) != (void *)a)
	{
		printf("    " RED "[%d] %s: must return the dst pointer\n" DEFAULT, i, desc);
		error -= 1;
	}
	memcpy(b, src, n);
	error += check_mem(i, desc, a, b, 64);
	return (error);
}

/* ft_memcpy(dst, NULL, 0) must return dst without reading src. */
static int	memcpy_null_case(int i)
{
	unsigned char	a[8];
	unsigned char	b[8];

	memset(a, 'A', 8);
	memset(b, 'A', 8);
	if (ft_memcpy(a, NULL, 0) != (void *)a)
	{
		printf("    " RED "[%d] ft_memcpy(dst, NULL, 0) must return dst\n" DEFAULT, i);
		return (-1);
	}
	return (check_mem(i, "ft_memcpy(dst, NULL, 0) returns dst and leaves it untouched", a, b, 8));
}

int main(void)
{
	int				error = 0;
	unsigned char	binary[64];
	size_t			k;

	for (k = 0; k < 64; k++)
		binary[k] = (unsigned char)(255 - k);

	error += memcpy_case(1, "ft_memcpy copies a string", "hello, world", 13);
	error += memcpy_case(2, "ft_memcpy copies 64 binary bytes", binary, 64);
	error += memcpy_case(3, "ft_memcpy copies embedded NULs (\"42\\0abc\")", "42\0abc", 7);
	error += memcpy_case(4, "ft_memcpy with n=0 leaves dst untouched", "ignored", 0);
	error += memcpy_null_case(5);

	return (error);
}
