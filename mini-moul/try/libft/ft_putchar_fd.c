#include "../try.h"

int	main(int argc, char **argv)
{
	int		c;
	int		fd;
	char	expected;
	int		same;

	try_args(argc, 1, 2, "putchar_fd <char> [fd, default 1]");
	c = try_char(argv[1], "char");
	fd = argc > 2 ? (int)try_long(argv[2], "fd", INT_MIN, INT_MAX) : 1;
	try_call("ft_putchar_fd");
	try_put_char(c);
	try_call_sep();
	printf("%d", fd);
	try_call_end();
	ft_putchar_fd((char)c, try_capture_fd(fd));
	expected = (char)c;
	same = try_capture_check(fd, &expected, 1);
	if (same < 0)
		return (0);
	return (try_verdict(same, "the expected output"));
}
