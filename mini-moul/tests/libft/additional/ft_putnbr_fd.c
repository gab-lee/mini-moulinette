#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

#define TMP_PATH ".ft_putnbr_fd_tmp"

static int	read_tmp(char *buf, size_t size)
{
	int		fd;
	ssize_t	n;

	fd = open(TMP_PATH, O_RDONLY);
	if (fd < 0)
		return (-1);
	n = read(fd, buf, size - 1);
	close(fd);
	if (n < 0)
		return (-1);
	buf[n] = '\0';
	return (0);
}

static int	putnbr_case(int i, char *desc, int n, char *expected)
{
	int		fd;
	char	buf[16];

	fd = open(TMP_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		printf("    " RED "[%d] %s: could not open temp file\n" DEFAULT, i, desc);
		return (-1);
	}
	ft_putnbr_fd(n, fd);
	close(fd);
	if (read_tmp(buf, sizeof(buf)) != 0)
	{
		printf("    " RED "[%d] %s: could not read back temp file\n" DEFAULT, i, desc);
		return (-1);
	}
	if (strcmp(buf, expected) == 0)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
		return (0);
	}
	printf("    " RED "[%d] %s: expected \"%s\", got \"%s\"\n" DEFAULT,
		i, desc, expected, buf);
	return (-1);
}

int main(void)
{
	int	error = 0;

	error += putnbr_case(1, "ft_putnbr_fd(0, fd) writes \"0\"", 0, "0");
	error += putnbr_case(2, "ft_putnbr_fd(42, fd) writes \"42\"", 42, "42");
	error += putnbr_case(3, "ft_putnbr_fd(-42, fd) writes \"-42\"", -42, "-42");
	error += putnbr_case(4, "ft_putnbr_fd(INT_MAX, fd) writes \"2147483647\"",
		INT_MAX, "2147483647");
	error += putnbr_case(5, "ft_putnbr_fd(INT_MIN, fd) writes \"-2147483648\"",
		INT_MIN, "-2147483648");
	/* Recursive digit-printers are commonly off-by-one on the recursion's
	   stop condition (e.g. `if (n > 10)` instead of `if (n >= 10)`), which
	   only misbehaves once the value itself, or some n/10^k reached while
	   descending, is exactly 10. None of the cases above ever hit that -
	   these do. */
	error += putnbr_case(6, "ft_putnbr_fd(10, fd) writes \"10\"", 10, "10");
	error += putnbr_case(7, "ft_putnbr_fd(100, fd) writes \"100\"", 100, "100");
	error += putnbr_case(8, "ft_putnbr_fd(1000, fd) writes \"1000\"", 1000, "1000");
	error += putnbr_case(9,
		"ft_putnbr_fd(105, fd) writes \"105\" (105 / 10 == 10)", 105, "105");
	error += putnbr_case(10, "ft_putnbr_fd(-10, fd) writes \"-10\"", -10, "-10");
	error += putnbr_case(11, "ft_putnbr_fd(10000000, fd) writes \"10000000\"",
		10000000, "10000000");

	unlink(TMP_PATH);
	return (error);
}
