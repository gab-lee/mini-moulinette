#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s;
	size_t	len;
	int		c;
	size_t	n;
	char	*mine;
	char	*ref;

	try_args(argc, 3, 3, "memchr <buffer> <char> <n>");
	s = try_str(argv[1], &len);
	c = try_char(argv[2], "char");
	n = try_size(argv[3], "n");
	try_call("ft_memchr");
	try_put_mem(s, s ? len + 1 : 0);
	try_call_sep();
	try_put_char(c);
	try_call_sep();
	printf("%zu", n);
	try_call_end();
	mine = ft_memchr(s, c, n);
	try_row("ft");
	try_put_found(s, mine);
	printf("\n");
	if (!s)
		return (try_no_ref("libc", "NULL argument"));
	if (n > len + 1)
		return (try_no_ref("libc", "n is past the end of the buffer"));
	ref = memchr(s, c, n);
	try_row("libc");
	try_put_found(s, ref);
	printf("\n");
	return (try_verdict(mine == ref, "libc"));
}
