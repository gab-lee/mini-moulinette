#include <ctype.h>
#include "../try.h"

int	main(int argc, char **argv)
{
	int	c;
	int	mine;
	int	ref;

	try_args(argc, 1, 1, "tolower <char>");
	c = try_char(argv[1], "char");
	try_call("ft_tolower");
	try_put_char(c);
	try_call_end();
	mine = ft_tolower(c);
	try_row("ft");
	try_put_char(mine);
	printf("\n");
	if (c < -1 || c > 255)
		return (try_no_ref("libc", "undefined for values outside -1..255"));
	ref = tolower(c);
	try_row("libc");
	try_put_char(ref);
	printf("\n");
	return (try_verdict(mine == ref, "libc"));
}
