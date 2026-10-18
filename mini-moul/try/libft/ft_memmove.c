#include "../try.h"

/* Both pointers point into one buffer, so the copy can overlap. */
int	main(int argc, char **argv)
{
	char	*mine;
	char	*ref;
	size_t	size;
	size_t	dst;
	size_t	src;
	size_t	n;
	void	*ret;

	try_args(argc, 4, 4, "memmove <buffer> <dst_offset> <src_offset> <len>");
	mine = try_buf(argv[1], 0, &size);
	ref = try_buf(argv[1], 0, NULL);
	if (!mine)
		try_usage("@null is not allowed for the buffer");
	dst = try_size(argv[2], "dst_offset");
	src = try_size(argv[3], "src_offset");
	n = try_size(argv[4], "len");
	if (dst > size || src > size)
		try_usage("offsets must be inside the buffer");
	printf(TRY_BOLD "ft_memmove" DEFAULT "(buffer + %zu, buffer + %zu, %zu) on ",
		dst, src, n);
	try_put_mem(mine, size);
	printf("\n");
	ret = ft_memmove(mine + dst, mine + src, n);
	try_row("ft");
	try_put_ret(ret, mine + dst);
	printf(", buffer ");
	try_put_mem(mine, size);
	printf("\n");
	if (n > size - dst || n > size - src)
		return (try_no_ref("libc", "len is past the end of the buffer"));
	memmove(ref + dst, ref + src, n);
	try_row("libc");
	printf("returned dst, buffer ");
	try_put_mem(ref, size);
	printf("\n");
	return (try_verdict(ret == mine + dst && memcmp(mine, ref, size) == 0,
			"libc"));
}
