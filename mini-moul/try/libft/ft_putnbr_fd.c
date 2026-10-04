#include "../try.h"

int	main(int argc, char **argv)
{
	int		n;
	int		fd;
	char	expected[16];
	int		same;

	try_args(argc, 1, 2, "putnbr_fd <int> [fd, default 1]");
	n = (int)try_long(argv[1], "n", INT_MIN, INT_MAX);
	fd = argc > 2 ? (int)try_long(argv[2], "fd", INT_MIN, INT_MAX) : 1;
	try_call("ft_putnbr_fd");
	printf("%d", n);
	try_call_sep();
	printf("%d", fd);
	try_call_end();
	ft_putnbr_fd(n, try_capture_fd(fd));
	snprintf(expected, sizeof(expected), "%d", n);
	same = try_capture_check(fd, expected, strlen(expected));
	if (same < 0)
		return (0);
	return (try_verdict(same, "the expected output"));
}
