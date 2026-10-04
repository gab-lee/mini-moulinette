#include "../try.h"

/* strlcpy is not in every libc, so a reference with BSD semantics */
static size_t	ref_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;

	len = strlen(src);
	if (size)
	{
		if (len < size - 1)
			size = len + 1;
		memcpy(dst, src, size - 1);
		dst[size - 1] = '\0';
	}
	return (len);
}

int	main(int argc, char **argv)
{
	char	*mine;
	char	*ref;
	size_t	size;
	char	*src;
	size_t	dstsize;
	size_t	ret_mine;
	size_t	ret_ref;

	try_args(argc, 3, 3, "strlcpy <dst> <src> <dstsize>");
	dstsize = try_size(argv[3], "dstsize");
	if (dstsize > 1 << 20)
		try_usage("dstsize must be at most 1048576 (the buffer is that big)");
	mine = try_buf(argv[1], dstsize, &size);
	ref = try_buf(argv[1], dstsize, NULL);
	src = try_str(argv[2], NULL);
	try_call("ft_strlcpy");
	try_put_mem(mine, size);
	try_call_sep();
	try_put_str(src);
	try_call_sep();
	printf("%zu", dstsize);
	try_call_end();
	ret_mine = ft_strlcpy(mine, src, dstsize);
	try_row("ft");
	printf("returned %zu, dst ", ret_mine);
	try_put_mem(mine, size);
	printf("\n");
	if (!mine || !src)
		return (try_no_ref("ref", "NULL argument"));
	ret_ref = ref_strlcpy(ref, src, dstsize);
	try_row("ref");
	printf("returned %zu, dst ", ret_ref);
	try_put_mem(ref, size);
	printf("\n");
	return (try_verdict(ret_mine == ret_ref && memcmp(mine, ref, size) == 0,
			"BSD strlcpy"));
}
