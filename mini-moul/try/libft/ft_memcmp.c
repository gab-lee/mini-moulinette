#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s1;
	char	*s2;
	size_t	len1;
	size_t	len2;
	size_t	n;
	int		mine;
	int		ref;

	try_args(argc, 3, 3, "memcmp <s1> <s2> <n>");
	s1 = try_str(argv[1], &len1);
	s2 = try_str(argv[2], &len2);
	n = try_size(argv[3], "n");
	try_call("ft_memcmp");
	try_put_mem(s1, s1 ? len1 + 1 : 0);
	try_call_sep();
	try_put_mem(s2, s2 ? len2 + 1 : 0);
	try_call_sep();
	printf("%zu", n);
	try_call_end();
	mine = ft_memcmp(s1, s2, n);
	try_row("ft");
	printf("%d\n", mine);
	if (!s1 || !s2)
		return (try_no_ref("libc", "NULL argument"));
	if (n > len1 + 1 || n > len2 + 1)
		return (try_no_ref("libc", "n is past the end of s1 or s2"));
	ref = memcmp(s1, s2, n);
	try_row("libc");
	printf("%d\n", ref);
	return (try_verdict(try_sign(mine) == try_sign(ref),
			"libc (same sign)"));
}
