#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s1;
	char	*s2;
	char	*mine;

	try_args(argc, 2, 2, "strjoin <s1> <s2>");
	s1 = try_str(argv[1], NULL);
	s2 = try_str(argv[2], NULL);
	try_call("ft_strjoin");
	try_put_str(s1);
	try_call_sep();
	try_put_str(s2);
	try_call_end();
	mine = ft_strjoin(s1, s2);
	try_row("ft");
	try_put_str(mine);
	printf("\n");
	free(mine);
	return (0);
}
