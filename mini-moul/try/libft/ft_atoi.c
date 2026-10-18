#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s;
	int		mine;
	int		ref;

	try_args(argc, 1, 1, "atoi <string>");
	s = try_str(argv[1], NULL);
	try_call("ft_atoi");
	try_put_str(s);
	try_call_end();
	mine = ft_atoi(s);
	try_row("ft");
	printf("%d\n", mine);
	if (!s)
		return (try_no_ref("libc", "NULL argument"));
	ref = atoi(s);
	try_row("libc");
	printf("%d\n", ref);
	return (try_verdict(mine == ref, "libc"));
}
