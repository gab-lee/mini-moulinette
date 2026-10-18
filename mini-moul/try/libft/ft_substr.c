#include "../try.h"

int	main(int argc, char **argv)
{
	char			*s;
	unsigned int	start;
	size_t			len;
	char			*mine;

	try_args(argc, 3, 3, "substr <string> <start> <len>");
	s = try_str(argv[1], NULL);
	start = (unsigned int)try_long(argv[2], "start", 0, UINT_MAX);
	len = try_size(argv[3], "len");
	try_call("ft_substr");
	try_put_str(s);
	try_call_sep();
	printf("%u", start);
	try_call_sep();
	printf("%zu", len);
	try_call_end();
	mine = ft_substr(s, start, len);
	try_row("ft");
	try_put_str(mine);
	printf("\n");
	free(mine);
	return (0);
}
