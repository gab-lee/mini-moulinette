#include <strings.h>
#include "../try.h"

int	main(int argc, char **argv)
{
	char	*mine;
	char	*ref;
	size_t	size;
	size_t	n;

	try_args(argc, 2, 2, "bzero <buffer> <n>");
	mine = try_buf(argv[1], 0, &size);
	ref = try_buf(argv[1], 0, NULL);
	n = try_size(argv[2], "n");
	try_call("ft_bzero");
	try_put_mem(mine, size);
	try_call_sep();
	printf("%zu", n);
	try_call_end();
	ft_bzero(mine, n);
	try_row("ft");
	printf("buffer ");
	try_put_mem(mine, size);
	printf("\n");
	if (!mine)
		return (try_no_ref("libc", "NULL argument"));
	if (n > size)
		return (try_no_ref("libc", "n is past the end of the buffer"));
	bzero(ref, n);
	try_row("libc");
	printf("buffer ");
	try_put_mem(ref, size);
	printf("\n");
	return (try_verdict(memcmp(mine, ref, size) == 0, "libc"));
}
