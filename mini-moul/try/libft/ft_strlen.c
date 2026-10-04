#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s;
	size_t	mine;
	size_t	ref;

	try_args(argc, 1, 1, "strlen <string>");
	s = try_str(argv[1], NULL);
	try_call("ft_strlen");
	try_put_str(s);
	try_call_end();
	mine = ft_strlen(s);
	try_row("ft");
	printf("%zu\n", mine);
	if (!s)
		return (try_no_ref("libc", "NULL argument"));
	ref = strlen(s);
	try_row("libc");
	printf("%zu\n", ref);
	return (try_verdict(mine == ref, "libc"));
}
