#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ft_strdup.c"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

int main(void)
{
	int		error = 0;
	char	src[] = "hello, world";
	char	*dup;

	dup = ft_strdup(src);
	if (dup == NULL)
	{
		printf("    " RED "[1] ft_strdup(\"hello, world\") returned NULL\n" DEFAULT);
		return (-1);
	}
	error += check_mem(1, "ft_strdup copies the content (incl. NUL)", dup, src, 13);
	if (dup == src)
	{
		printf("    " RED "[2] ft_strdup returned the same pointer: it must allocate new memory\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] ft_strdup allocates new memory\n" DEFAULT);
	dup[0] = 'H';
	if (src[0] != 'h')
	{
		printf("    " RED "[3] modifying the copy changed the original\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] the copy is independent of the original\n" DEFAULT);
	free(dup);
	dup = ft_strdup("");
	if (dup == NULL || dup[0] != '\0')
	{
		printf("    " RED "[4] ft_strdup(\"\") must return an allocated empty string\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [4] ft_strdup(\"\") returns an empty string\n" DEFAULT);
	free(dup);

	return (error);
}
