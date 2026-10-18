#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s;
	int		c;
	char	*mine;
	char	*ref;

	try_args(argc, 2, 2, "strchr <string> <char>");
	s = try_str(argv[1], NULL);
	c = try_char(argv[2], "char");
	try_call("ft_strchr");
	try_put_str(s);
	try_call_sep();
	try_put_char(c);
	try_call_end();
	mine = ft_strchr(s, c);
	try_row("ft");
	try_put_found(s, mine);
	printf("\n");
	if (!s)
		return (try_no_ref("libc", "NULL argument"));
	ref = strchr(s, c);
	try_row("libc");
	try_put_found(s, ref);
	printf("\n");
	return (try_verdict(mine == ref, "libc"));
}
