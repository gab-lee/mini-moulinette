#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s;
	char	*mine;
	char	*expected;
	size_t	i;
	int		same;

	try_args(argc, 2, 2, "strmapi <string> <" TRY_MAP_NAMES ">");
	s = try_str(argv[1], NULL);
	*try_map_choice() = try_map_id(argv[2]);
	try_call("ft_strmapi");
	try_put_str(s);
	try_call_sep();
	printf("%s", argv[2]);
	try_call_end();
	mine = ft_strmapi(s, try_map_apply);
	try_row("ft");
	try_put_str(mine);
	printf("\n");
	if (!s)
	{
		free(mine);
		return (0);
	}
	expected = try_alloc(strlen(s) + 1);
	i = 0;
	while (s[i])
	{
		expected[i] = try_map_apply((unsigned int)i, s[i]);
		i++;
	}
	try_row("expect");
	try_put_str(expected);
	printf("\n");
	same = mine && strcmp(mine, expected) == 0;
	free(mine);
	return (try_verdict(same, "the expected string"));
}
