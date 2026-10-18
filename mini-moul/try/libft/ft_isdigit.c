#include <ctype.h>
#include "../try.h"

int	main(int argc, char **argv)
{
	int	c;
	int	mine;
	int	ref;

	try_args(argc, 1, 1, "isdigit <char>");
	c = try_char(argv[1], "char");
	try_call("ft_isdigit");
	try_put_char(c);
	try_call_end();
	mine = ft_isdigit(c);
	try_row("ft");
	printf("%d\n", mine);
	if (c < -1 || c > 255)
		return (try_no_ref("libc", "undefined for values outside -1..255"));
	ref = isdigit(c);
	try_row("libc");
	printf("%d\n", ref);
	return (try_verdict(!mine == !ref, "libc (both zero or both non-zero)"));
}
