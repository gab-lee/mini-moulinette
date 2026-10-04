#include "../try.h"

/* strlcat is not in every libc, so a reference with BSD semantics */
static size_t	ref_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dlen;
	size_t	slen;
	size_t	i;

	dlen = 0;
	slen = strlen(src);
	while (dlen < size && dst[dlen])
		dlen++;
	if (dlen == size)
		return (size + slen);
	i = 0;
	while (src[i] && dlen + i + 1 < size)
	{
		dst[dlen + i] = src[i];
		i++;
	}
	dst[dlen + i] = '\0';
	return (dlen + slen);
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

	try_args(argc, 3, 3, "strlcat <dst> <src> <dstsize>");
	dstsize = try_size(argv[3], "dstsize");
	if (dstsize > 1 << 20)
		try_usage("dstsize must be at most 1048576 (the buffer is that big)");
	mine = try_buf(argv[1], dstsize, &size);
	ref = try_buf(argv[1], dstsize, NULL);
	src = try_str(argv[2], NULL);
	try_call("ft_strlcat");
	try_put_mem(mine, size);
	try_call_sep();
	try_put_str(src);
	try_call_sep();
	printf("%zu", dstsize);
	try_call_end();
	ret_mine = ft_strlcat(mine, src, dstsize);
	try_row("ft");
	printf("returned %zu, dst ", ret_mine);
	try_put_mem(mine, size);
	printf("\n");
	if (!mine || !src)
		return (try_no_ref("ref", "NULL argument"));
	ret_ref = ref_strlcat(ref, src, dstsize);
	try_row("ref");
	printf("returned %zu, dst ", ret_ref);
	try_put_mem(ref, size);
	printf("\n");
	return (try_verdict(ret_mine == ret_ref && memcmp(mine, ref, size) == 0,
			"BSD strlcat"));
}
