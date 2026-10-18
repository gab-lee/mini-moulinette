#include "../try.h"

static void	iter(unsigned int i, char *c)
{
	*c = try_map_apply(i, *c);
}

int	main(int argc, char **argv)
{
	char	*s;
	char	*expected;
	size_t	size;
	size_t	i;

	try_args(argc, 2, 2, "striteri <string> <" TRY_MAP_NAMES ">");
	s = try_buf(argv[1], 0, &size);
	*try_map_choice() = try_map_id(argv[2]);
	try_call("ft_striteri");
	try_put_str(s);
	try_call_sep();
	printf("%s", argv[2]);
	try_call_end();
	expected = try_buf(argv[1], 0, NULL);
	ft_striteri(s, iter);
	try_row("ft");
	try_put_str(s);
	printf("\n");
	if (!s)
		return (0);
	i = 0;
	while (expected[i])
	{
		expected[i] = try_map_apply((unsigned int)i, expected[i]);
		i++;
	}
	try_row("expect");
	try_put_str(expected);
	printf("\n");
	return (try_verdict(memcmp(s, expected, size) == 0, "the expected string"));
}
