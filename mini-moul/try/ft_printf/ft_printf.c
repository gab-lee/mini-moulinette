#include "../try.h"
#include "../../utils/printf_compare.h"
#include "../../tests/ft_printf/ft_printf_proto.h"

/*
** mini --try printf <format> [argument ...]
**
** Calls ft_printf and the real printf with the same format and arguments,
** each in a forked child (so a crash or an endless loop is reported here),
** and compares the bytes written and the return value. The format is
** read for its conversions, which tell how to read the arguments:
**   %c            a character (see "char" in the help)
**   %s            a string, @null passes NULL
**   %p            a number (decimal or 0x...), @null passes NULL
**   %d %i         a number from INT_MIN to INT_MAX
**   %u %x %X      a number from INT_MIN to UINT_MAX (negative wraps)
** Flags, width and precision are skipped over. At most 8 arguments.
*/

#define TRY_PF_MAX 8
#define TRY_PF_USAGE "printf <format> [argument ...]  (one argument per conversion)"

typedef int	(*t_printf_fn)(const char *, ...);

/* The arguments travel as longs: every variadic argument of ft_printf is an
** int or a pointer, and both read back from a long-sized slot. */
static int	call_n(t_printf_fn fn, const char *f, const long *a, int n)
{
	if (n == 0)
		return (fn(f));
	if (n == 1)
		return (fn(f, a[0]));
	if (n == 2)
		return (fn(f, a[0], a[1]));
	if (n == 3)
		return (fn(f, a[0], a[1], a[2]));
	if (n == 4)
		return (fn(f, a[0], a[1], a[2], a[3]));
	if (n == 5)
		return (fn(f, a[0], a[1], a[2], a[3], a[4]));
	if (n == 6)
		return (fn(f, a[0], a[1], a[2], a[3], a[4], a[5]));
	if (n == 7)
		return (fn(f, a[0], a[1], a[2], a[3], a[4], a[5], a[6]));
	return (fn(f, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]));
}

/* The conversion letter that takes an argument, or 0 for none. spec points
** after the '%'; *end gets the position after the conversion. */
static char	next_conv(const char *spec, const char **end)
{
	while (*spec && strchr("-0# +.123456789", *spec))
		spec++;
	*end = spec;
	if (*spec)
		*end = spec + 1;
	if (*spec && strchr("cspdiuxX", *spec))
		return (*spec);
	return (0);
}

static long	pointer_arg(const char *arg)
{
	char				*end;
	unsigned long long	v;

	if (try_is_null(arg))
		return (0);
	errno = 0;
	v = strtoull(arg, &end, 0);
	if (end == arg || *end || errno)
		try_usage("a %p argument must be a number (42, 0x2a) or @null");
	return ((long)(uintptr_t)v);
}

static void	put_arg(char conv, long v)
{
	if (conv == 'c')
		try_put_char((int)v);
	else if (conv == 's')
		try_put_str((const char *)(uintptr_t)v);
	else if (conv == 'p')
	{
		if (v == 0)
			printf("NULL");
		else
			printf("0x%lx", v);
	}
	else if (conv == 'd' || conv == 'i')
		printf("%d", (int)v);
	else
		printf("%u", (unsigned int)v);
}

static void	put_result(const char *label, char *out, long len, int ret)
{
	try_row(label);
	if (!out)
	{
		if (len == -1)
			printf(RED "memory error (see the Memory fail line)\n" DEFAULT);
		else if (len == SIGALRM)
			printf(RED "timed out after %ds (infinite loop?)\n" DEFAULT,
				PF_TIMEOUT);
		else if (len > 0)
			printf(RED "crashed: %s\n" DEFAULT, strsignal((int)len));
		else
			printf(RED "exited before returning\n" DEFAULT);
		return ;
	}
	try_put_mem(out, len);
	printf(" (%ld byte%s), returned %d\n", len, len == 1 ? "" : "s", ret);
}

int	main(int argc, char **argv)
{
	char		convs[TRY_PF_MAX];
	long		args[TRY_PF_MAX];
	const char	*fmt;
	const char	*p;
	char		conv;
	size_t		flen;
	int			n;
	int			i;
	int			ret_mine;
	int			ret_ref;
	char		*out_mine;
	char		*out_ref;
	long		len_mine;
	long		len_ref;
	int			same;

	try_args(argc, 1, TRY_PF_MAX + 1, TRY_PF_USAGE);
	fmt = try_str(argv[1], &flen);
	n = 0;
	p = fmt;
	while (fmt && *p)
	{
		if (*p++ != '%')
			continue ;
		conv = next_conv(p, &p);
		if (!conv)
			continue ;
		if (n == TRY_PF_MAX)
			try_usage("too many conversions (at most 8)");
		convs[n++] = conv;
	}
	if (n != argc - 2)
	{
		printf(RED "the format takes %d argument%s but %d %s given\n" DEFAULT,
			n, n == 1 ? "" : "s", argc - 2, argc - 2 == 1 ? "was" : "were");
		try_usage(NULL);
	}
	i = 0;
	while (i < n)
	{
		if (convs[i] == 'c')
			args[i] = try_char(argv[i + 2], "%c argument");
		else if (convs[i] == 's')
			args[i] = (long)(uintptr_t)try_str(argv[i + 2], NULL);
		else if (convs[i] == 'p')
			args[i] = pointer_arg(argv[i + 2]);
		else if (convs[i] == 'd' || convs[i] == 'i')
			args[i] = try_long(argv[i + 2], "an argument", INT_MIN, INT_MAX);
		else
			args[i] = (long)(unsigned int)try_long(argv[i + 2], "an argument",
					INT_MIN, UINT_MAX);
		i++;
	}

	try_call("ft_printf");
	try_put_mem(fmt, flen);
	i = 0;
	while (i < n)
	{
		try_call_sep();
		put_arg(convs[i], args[i]);
		i++;
	}
	try_call_end();

	PF_RUN(call_n, ret_mine, out_mine, len_mine, ft_printf, fmt, args, n);
	put_result("ft", out_mine, len_mine, ret_mine);
	if (!fmt)
	{
		free(out_mine);
		return (try_no_ref("libc", "NULL format"));
	}
	PF_RUN(call_n, ret_ref, out_ref, len_ref, real_printf, fmt, args, n);
	put_result("printf", out_ref, len_ref, ret_ref);
	if (!out_mine || !out_ref)
	{
		free(out_mine);
		free(out_ref);
		return (try_verdict(0, "printf"));
	}
	same = (len_mine == len_ref) && (len_ref == 0
			|| memcmp(out_mine, out_ref, len_ref) == 0);
	if (!same)
		printf("  " GREY "%-7s" DEFAULT RED "the output differs\n" DEFAULT, "");
	if (ret_mine != ret_ref)
		printf("  " GREY "%-7s" DEFAULT RED "the return value differs (%d, expected %d)\n"
			DEFAULT, "", ret_mine, ret_ref);
	same = same && ret_mine == ret_ref;
	free(out_mine);
	free(out_ref);
	return (try_verdict(same, "printf"));
}
