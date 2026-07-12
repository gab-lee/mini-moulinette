#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

#define TMP_PATH ".ft_putendl_fd_tmp"

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

static int	putendl_case(int i, char *desc, char *s, char *expected)
{
	int		fd;
	char	buf[64];

	fd = open(TMP_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		printf("    " RED "[%d] %s: could not open temp file\n" DEFAULT, i, desc);
		return (-1);
	}
	ft_putendl_fd(s, fd);
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
	int		error = 0;
	char	hello[] = "hello, world";

	error += putendl_case(1, "ft_putendl_fd(\"hello, world\", fd) writes the string plus a newline",
		hello, "hello, world\n");
	error += putendl_case(2, "ft_putendl_fd(\"\", fd) writes only a newline", "", "\n");

	unlink(TMP_PATH);
	return (error);
}
