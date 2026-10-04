#include "../try.h"

/* strnstr is not in every libc, so a reference with BSD semantics */
static char	*ref_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	nlen;

	nlen = strlen(needle);
	if (!nlen)
		return ((char *)haystack);
	while (*haystack && len >= nlen)
	{
		if (strncmp(haystack, needle, nlen) == 0)
			return ((char *)haystack);
		haystack++;
		len--;
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	char	*haystack;
	char	*needle;
	size_t	len;
	char	*mine;
	char	*ref;

	try_args(argc, 3, 3, "strnstr <haystack> <needle> <len>");
	haystack = try_str(argv[1], NULL);
	needle = try_str(argv[2], NULL);
	len = try_size(argv[3], "len");
	try_call("ft_strnstr");
	try_put_str(haystack);
	try_call_sep();
	try_put_str(needle);
	try_call_sep();
	printf("%zu", len);
	try_call_end();
	mine = ft_strnstr(haystack, needle, len);
	try_row("ft");
	try_put_found(haystack, mine);
	printf("\n");
	if (!haystack || !needle)
		return (try_no_ref("ref", "NULL argument"));
	ref = ref_strnstr(haystack, needle, len);
	try_row("ref");
	try_put_found(haystack, ref);
	printf("\n");
	return (try_verdict(mine == ref, "BSD strnstr"));
}
