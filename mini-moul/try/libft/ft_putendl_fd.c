#include "../try.h"

int	main(int argc, char **argv)
{
	char	*s;
	size_t	len;
	int		fd;
	char	*expected;
	int		same;

	try_args(argc, 1, 2, "putendl_fd <string> [fd, default 1]");
	s = try_str(argv[1], &len);
	fd = argc > 2 ? (int)try_long(argv[2], "fd", INT_MIN, INT_MAX) : 1;
	try_call("ft_putendl_fd");
	try_put_str(s);
	try_call_sep();
	printf("%d", fd);
	try_call_end();
	ft_putendl_fd(s, try_capture_fd(fd));
	expected = NULL;
	if (s)
	{
		len = strlen(s);
		expected = try_alloc(len + 2);
		memcpy(expected, s, len);
		expected[len++] = '\n';
	}
	same = try_capture_check(fd, expected, len);
	if (same < 0)
		return (0);
	return (try_verdict(same, "the expected output"));
}
