#include "../try.h"

int	main(int argc, char **argv)
{
	char	*mine;
	char	*ref;
	size_t	size;
	int		c;
	size_t	n;
	void	*ret;

	try_args(argc, 3, 3, "memset <buffer> <char> <len>");
	mine = try_buf(argv[1], 0, &size);
	ref = try_buf(argv[1], 0, NULL);
	c = try_char(argv[2], "char");
	n = try_size(argv[3], "len");
	try_call("ft_memset");
	try_put_mem(mine, size);
	try_call_sep();
	try_put_char(c);
	try_call_sep();
	printf("%zu", n);
	try_call_end();
	ret = ft_memset(mine, c, n);
	try_row("ft");
	try_put_ret(ret, mine);
	printf(", buffer ");
	try_put_mem(mine, size);
	printf("\n");
	if (!mine)
		return (try_no_ref("libc", "NULL argument"));
	if (n > size)
		return (try_no_ref("libc", "len is past the end of the buffer"));
	memset(ref, c, n);
	try_row("libc");
	printf("returned dst, buffer ");
	try_put_mem(ref, size);
	printf("\n");
	return (try_verdict(ret == mine && memcmp(mine, ref, size) == 0, "libc"));
}
