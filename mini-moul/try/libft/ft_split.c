#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s;
	int		c;
	char	**mine;
	size_t	i;

	try_args(argc, 2, 2, "split <string> <char>");
	s = try_str(argv[1], NULL);
	c = try_char(argv[2], "char");
	try_call("ft_split");
	try_put_str(s);
	try_call_sep();
	try_put_char(c);
	try_call_end();
	mine = ft_split(s, (char)c);
	try_row("ft");
	if (!mine)
	{
		printf("NULL\n");
		return (0);
	}
	printf("[");
	i = 0;
	while (mine[i])
	{
		if (i)
			printf(", ");
		try_put_str(mine[i]);
		free(mine[i++]);
	}
	printf("] (%zu word%s, then NULL)\n", i, i == 1 ? "" : "s");
	free(mine);
	return (0);
}
