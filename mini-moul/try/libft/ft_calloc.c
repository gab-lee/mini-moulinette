#include "../try.h"

static void	put_block(void *p, size_t count, size_t size, int *zeroed)
{
	size_t	i;

	*zeroed = 1;
	if (!p)
	{
		printf("NULL\n");
		return ;
	}
	if (size && count > SIZE_MAX / size)
	{
		printf("a block (size unknown, count * size overflows)\n");
		return ;
	}
	i = 0;
	while (i < count * size && ((unsigned char *)p)[i] == 0)
		i++;
	*zeroed = (i == count * size);
	printf("a block of %zu bytes, %s\n", count * size,
		*zeroed ? "all zero" : "NOT all zero");
}

int	main(int argc, char **argv)
{
	size_t	count;
	size_t	size;
	void	*mine;
	void	*ref;
	int		mine_zeroed;
	int		ref_zeroed;
	int		same;

	try_args(argc, 2, 2, "calloc <count> <size>");
	count = try_size(argv[1], "count");
	size = try_size(argv[2], "size");
	try_call("ft_calloc");
	printf("%zu", count);
	try_call_sep();
	printf("%zu", size);
	try_call_end();
	mine = ft_calloc(count, size);
	try_row("ft");
	put_block(mine, count, size, &mine_zeroed);
	ref = calloc(count, size);
	try_row("libc");
	put_block(ref, count, size, &ref_zeroed);
	same = (!mine == !ref) && mine_zeroed;
	free(mine);
	free(ref);
	return (try_verdict(same, "libc (both NULL, or both a zeroed block)"));
}
