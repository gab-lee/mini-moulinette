#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s;
	char	*mine;
	int		same;

	try_args(argc, 1, 1, "strdup <string>");
	s = try_str(argv[1], NULL);
	try_call("ft_strdup");
	try_put_str(s);
	try_call_end();
	mine = ft_strdup(s);
	try_row("ft");
	try_put_str(mine);
	if (mine && mine == s)
		printf(" (the same pointer, not a copy)");
	printf("\n");
	if (!s)
	{
		free(mine);
		return (try_no_ref("libc", "NULL argument"));
	}
	try_row("libc");
	try_put_str(s);
	printf("\n");
	same = mine && mine != s && strcmp(mine, s) == 0;
	if (mine != s)
		free(mine);
	return (try_verdict(same, "libc"));
}
