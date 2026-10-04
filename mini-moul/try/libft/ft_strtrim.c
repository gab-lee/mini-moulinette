#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s1;
	char	*set;
	char	*mine;

	try_args(argc, 2, 2, "strtrim <string> <set>");
	s1 = try_str(argv[1], NULL);
	set = try_str(argv[2], NULL);
	try_call("ft_strtrim");
	try_put_str(s1);
	try_call_sep();
	try_put_str(set);
	try_call_end();
	mine = ft_strtrim(s1, set);
	try_row("ft");
	try_put_str(mine);
	printf("\n");
	free(mine);
	return (0);
}
