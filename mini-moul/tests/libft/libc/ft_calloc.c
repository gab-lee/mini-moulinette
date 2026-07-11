#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

int main(void)
{
	int				error = 0;
	unsigned char	zeros[64];
	unsigned char	*p;
	void			*q;

	memset(zeros, 0, 64);
	p = (unsigned char *)ft_calloc(16, 4);
	if (p == NULL)
	{
		printf("    " RED "[1] ft_calloc(16, 4) returned NULL\n" DEFAULT);
		error -= 1;
	}
	else
	{
		error += check_mem(1, "ft_calloc(16, 4) memory is fully zeroed", p, zeros, 64);
		p[0] = 42;
		p[63] = 42;
		printf("  " GREEN CHECKMARK GREY " [2] ft_calloc memory is writable\n" DEFAULT);
		free(p);
	}
	q = ft_calloc(0, 8);
	printf("  " GREEN CHECKMARK GREY " [3] ft_calloc(0, 8) did not crash\n" DEFAULT);
	free(q);
	q = ft_calloc(SIZE_MAX, SIZE_MAX);
	if (q == NULL)
		printf("  " GREEN CHECKMARK GREY " [4] ft_calloc(SIZE_MAX, SIZE_MAX) returns NULL (overflow handled)\n" DEFAULT);
	else
	{
		printf("    " YELLOW "[!] [4] ft_calloc(SIZE_MAX, SIZE_MAX): count * size overflow not handled, real calloc returns NULL\n" DEFAULT);
		free(q);
	}

	return (error);
}
