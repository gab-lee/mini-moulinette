#include "../try.h"

int	main(int argc, char **argv)
{
	int		n;
	char	*mine;
	char	ref[16];
	int		same;

	try_args(argc, 1, 1, "itoa <int>");
	n = (int)try_long(argv[1], "n", INT_MIN, INT_MAX);
	try_call("ft_itoa");
	printf("%d", n);
	try_call_end();
	mine = ft_itoa(n);
	try_row("ft");
	try_put_str(mine);
	printf("\n");
	snprintf(ref, sizeof(ref), "%d", n);
	try_row("printf");
	try_put_str(ref);
	printf("\n");
	same = mine && strcmp(mine, ref) == 0;
	free(mine);
	return (try_verdict(same, "printf(\"%d\")"));
}
