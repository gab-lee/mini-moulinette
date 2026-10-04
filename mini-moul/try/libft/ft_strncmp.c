#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s1;
	char	*s2;
	size_t	n;
	int		mine;
	int		ref;

	try_args(argc, 3, 3, "strncmp <s1> <s2> <n>");
	s1 = try_str(argv[1], NULL);
	s2 = try_str(argv[2], NULL);
	n = try_size(argv[3], "n");
	try_call("ft_strncmp");
	try_put_str(s1);
	try_call_sep();
	try_put_str(s2);
	try_call_sep();
	printf("%zu", n);
	try_call_end();
	mine = ft_strncmp(s1, s2, n);
	try_row("ft");
	printf("%d\n", mine);
	if (!s1 || !s2)
		return (try_no_ref("libc", "NULL argument"));
	ref = strncmp(s1, s2, n);
	try_row("libc");
	printf("%d\n", ref);
	return (try_verdict(try_sign(mine) == try_sign(ref),
			"libc (same sign)"));
}
