#include <stdio.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

/* dst_off/src_off are offsets into the same buffer so overlap is real */
static int	memmove_case(int i, char *desc, size_t dst_off, size_t src_off, size_t n)
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
	if (ft_memmove(a + dst_off, a + src_off, n) != (void *)(a + dst_off))
	{
		printf("    " RED "[%d] %s: must return the dst pointer\n" DEFAULT, i, desc);
		error -= 1;
	}
	memmove(b + dst_off, b + src_off, n);
	error += check_mem(i, desc, a, b, 64);
	return (error);
}

int main(void)
{
	int error = 0;

	error += memmove_case(1, "ft_memmove non-overlapping copy", 40, 0, 16);
	error += memmove_case(2, "ft_memmove overlap, dst after src (must copy backwards)", 8, 0, 32);
	error += memmove_case(3, "ft_memmove overlap, dst before src", 0, 8, 32);
	error += memmove_case(4, "ft_memmove dst == src", 16, 16, 16);
	error += memmove_case(5, "ft_memmove with n=0 leaves buffer untouched", 0, 32, 0);

	return (error);
}
