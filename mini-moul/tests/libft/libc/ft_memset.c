#include <stdio.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

static int	memset_case(int i, char *desc, int c, size_t n)
{
	unsigned char	a[64];
	unsigned char	b[64];
	size_t			k;
	int				error = 0;

	for (k = 0; k < 64; k++)
	{
		a[k] = (unsigned char)k;
		b[k] = (unsigned char)k;
	}
	if (ft_memset(a, c, n) != (void *)a)
	{
		printf("    " RED "[%d] %s: must return the dst pointer\n" DEFAULT, i, desc);
		error -= 1;
	}
	memset(b, c, n);
	error += check_mem(i, desc, a, b, 64);
	return (error);
}

int main(void)
{
	int error = 0;

	error += memset_case(1, "ft_memset fills 16 bytes with 'A'", 'A', 16);
	error += memset_case(2, "ft_memset fills the whole buffer with 0", 0, 64);
	error += memset_case(3, "ft_memset fills 32 bytes with 255", 255, 32);
	error += memset_case(4, "ft_memset with n=0 leaves buffer untouched", 'X', 0);
	error += memset_case(5, "ft_memset with c=300 (truncates to a byte)", 300, 16);

	return (error);
}
