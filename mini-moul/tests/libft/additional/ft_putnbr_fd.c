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

	unlink(TMP_PATH);
	return (error);
}
