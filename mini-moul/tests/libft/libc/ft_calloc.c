#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"
#include "../../../utils/alloc_check.h"

/* Subject rule: if nmemb or size is 0, return a unique pointer that can be
** passed to free() - NULL is not accepted. */
static int	zero_case(int i, char *desc, size_t count, size_t size)
{
	void	*q;

	q = ft_calloc(count, size);
	if (q == NULL)
	{
		printf("    " RED "[%d] %s returned NULL; the subject requires a unique pointer that can be passed to free()\n" DEFAULT,
			i, desc);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s returned a freeable pointer\n" DEFAULT, i, desc);
	free(q);
	return (0);
}

/* count * size overflows (or cannot be allocated): the real calloc returns
** NULL instead of allocating a wrapped-around size. */
static int	null_case(int i, char *desc, size_t count, size_t size)
{
	void	*q;

	q = ft_calloc(count, size);
	if (q == NULL)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s returns NULL\n" DEFAULT, i, desc);
		return (0);
	}
	printf("    " RED "[%d] %s: expected NULL (count * size overflows), got a pointer\n" DEFAULT,
		i, desc);
	free(q);
	return (-1);
}

int main(void)
{
	int				error = 0;
	unsigned char	zeros[64];
	unsigned char	*p;

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
		error += check_alloc_size(3, "ft_calloc(16, 4)", p, 64);
		free(p);
	}
	p = (unsigned char *)ft_calloc(2, 2);
	if (p == NULL)
	{
		printf("    " RED "[4] ft_calloc(2, 2) returned NULL\n" DEFAULT);
		error -= 1;
	}
	else
	{
		error += check_mem(4, "ft_calloc(2, 2) memory is fully zeroed", p, zeros, 4);
		error += check_alloc_size(5, "ft_calloc(2, 2)", p, 4);
		free(p);
	}
	error += zero_case(6, "ft_calloc(0, 8)", 0, 8);
	error += zero_case(7, "ft_calloc(8, 0)", 8, 0);
	error += zero_case(8, "ft_calloc(0, 0)", 0, 0);
	error += zero_case(9, "ft_calloc(0, -5)", 0, (size_t)-5);
	error += zero_case(10, "ft_calloc(-5, 0)", (size_t)-5, 0);
	error += null_case(11, "ft_calloc(SIZE_MAX, SIZE_MAX)", SIZE_MAX, SIZE_MAX);
	error += null_case(12, "ft_calloc(INT_MAX, INT_MAX)", INT_MAX, INT_MAX);
	error += null_case(13, "ft_calloc(INT_MIN, INT_MIN)", (size_t)INT_MIN, (size_t)INT_MIN);
	error += null_case(14, "ft_calloc(-5, -5)", (size_t)-5, (size_t)-5);
	error += null_case(15, "ft_calloc(3, -5)", 3, (size_t)-5);
	error += null_case(16, "ft_calloc(-5, 3)", (size_t)-5, 3);

	return (error);
}
