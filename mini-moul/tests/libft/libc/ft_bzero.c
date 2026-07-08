#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "../../../../ft_bzero.c"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

static int	bzero_case(int i, char *desc, size_t n)
{
	unsigned char	a[64];
	unsigned char	b[64];
	size_t			k;

	for (k = 0; k < 64; k++)
	{
		a[k] = (unsigned char)(k + 1);
		b[k] = (unsigned char)(k + 1);
	}
	ft_bzero(a, n);
	bzero(b, n);
	return (check_mem(i, desc, a, b, 64));
}

int main(void)
{
	int error = 0;

	error += bzero_case(1, "ft_bzero zeroes 16 bytes", 16);
	error += bzero_case(2, "ft_bzero zeroes the whole buffer", 64);
	error += bzero_case(3, "ft_bzero with n=0 leaves buffer untouched", 0);
	error += bzero_case(4, "ft_bzero zeroes a single byte", 1);

	return (error);
}
