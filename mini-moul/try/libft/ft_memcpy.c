#include "../try.h"

int	main(int argc, char **argv)
{
	char	*mine;
	char	*ref;
	size_t	size;
	char	*src;
	size_t	src_len;
	size_t	n;
	void	*ret;

	try_args(argc, 3, 3, "memcpy <dst> <src> <n>");
	mine = try_buf(argv[1], 0, &size);
	ref = try_buf(argv[1], 0, NULL);
	src = try_str(argv[2], &src_len);
	n = try_size(argv[3], "n");
	try_call("ft_memcpy");
	try_put_mem(mine, size);
	try_call_sep();
	try_put_mem(src, src ? src_len + 1 : 0);
	try_call_sep();
	printf("%zu", n);
	try_call_end();
	ret = ft_memcpy(mine, src, n);
	try_row("ft");
	try_put_ret(ret, mine);
	printf(", dst ");
	try_put_mem(mine, size);
	printf("\n");
	if (!mine || !src)
		return (try_no_ref("libc", "NULL argument"));
	if (n > size || n > src_len + 1)
		return (try_no_ref("libc", "n is past the end of dst or src"));
	memcpy(ref, src, n);
	try_row("libc");
	printf("returned dst, dst ");
	try_put_mem(ref, size);
	printf("\n");
	return (try_verdict(ret == mine && memcmp(mine, ref, size) == 0, "libc"));
}
