#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** Values and shapes that go wrong in real ft_printf implementations: byte
** values outside 0..127, conversions that look like format text inside an
** argument, runs of '%%', digit-count boundaries, NULs in the middle of the
** output, long strings and long formats, and the return value on each.
*/

static char *repeat(const char *unit, int times)
{
	size_t len;
	char *s;
	int i;

	len = strlen(unit);
	s = malloc(len * times + 1);
	if (s == NULL)
		exit(1);
	i = 0;
	while (i < times)
	{
		memcpy(s + i * len, unit, len);
		i++;
	}
	s[len * times] = '\0';
	return (s);
}

int main(void)
{
	int local_var;
	char *big;
	char *huge;
	char *pct_fmt;
	char *lit_fmt;
	int error;
	int ret_mine;
	int ret_ref;
	char *out_mine;
	char *out_ref;
	long len_mine;
	long len_ref;

	error = 0;
	local_var = 0;
	big = repeat("a", 10000);
	huge = repeat("b", 100000);
	pct_fmt = repeat("%%", 1000);
	lit_fmt = repeat("literal text ", 200);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", -1);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", -1);
	error += check_printf(1, "ft_printf(\"%c\", -1) (wraps to byte 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", 255);
	error += check_printf(2, "ft_printf(\"%c\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", 256);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", 256);
	error += check_printf(3, "ft_printf(\"%c\", 256) (wraps to the NUL byte)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", 127);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", 127);
	error += check_printf(4, "ft_printf(\"%c\", 127) (DEL)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", '\n');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", '\n');
	error += check_printf(5, "ft_printf(\"%c\", '\\n')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c%c", 0, 'a');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c%c", 0, 'a');
	error += check_printf(6, "ft_printf(\"%c%c\", 0, 'a') (a NUL then a letter)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c%c%c", 0, 0, 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c%c%c", 0, 0, 0);
	error += check_printf(7, "ft_printf(\"%c%c%c\", 0, 0, 0) (three NULs are three bytes)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c %c %c", 'a', 'b', 'c');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c %c %c", 'a', 'b', 'c');
	error += check_printf(8, "ft_printf(\"%c %c %c\", 'a', 'b', 'c')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s", big);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s", big);
	error += check_printf(9, "ft_printf(\"%s\", big) (10000 characters)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s", huge);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s", huge);
	error += check_printf(10, "ft_printf(\"%s\", huge) (100000 characters)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s%s%s", big, big, big);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s%s%s", big, big, big);
	error += check_printf(11, "ft_printf(\"%s%s%s\", big, big, big) (30000 bytes, so the return value is large)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s%s%s", "a", "", "c");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s%s%s", "a", "", "c");
	error += check_printf(12, "ft_printf(\"%s%s%s\", \"a\", \"\", \"c\") (an empty string in the middle)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s%s", "", "");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s%s", "", "");
	error += check_printf(13, "ft_printf(\"%s%s\", \"\", \"\") (nothing printed, returns 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s", "%d %s %c %%");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s", "%d %s %c %%");
	error += check_printf(14, "ft_printf(\"%s\", \"%d %s %c %%\") (conversions inside an argument are plain text)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "a%sb", NULL);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "a%sb", NULL);
	error += check_printf(15, "ft_printf(\"a%sb\", NULL)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s %s", "(null)", NULL);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s %s", "(null)", NULL);
	error += check_printf(16, "ft_printf(\"%s %s\", \"(null)\", NULL) (the text (null) and a NULL pointer print the same)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s", "tab\tnew\nline");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s", "tab\tnew\nline");
	error += check_printf(17, "ft_printf(\"%s\", \"tab\\tnew\\nline\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%d", INT_MIN + 1);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%d", INT_MIN + 1);
	error += check_printf(18, "ft_printf(\"%d\", INT_MIN + 1)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%d", -1);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%d", -1);
	error += check_printf(19, "ft_printf(\"%d\", -1)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%d %d %d", 10, 100, 1000000);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%d %d %d", 10, 100, 1000000);
	error += check_printf(20, "ft_printf(\"%d %d %d\", 10, 100, 1000000) (digit-count boundaries)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%d%d%d", 1, 2, 3);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%d%d%d", 1, 2, 3);
	error += check_printf(21, "ft_printf(\"%d%d%d\", 1, 2, 3) (no separators)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%d %d", 1000000000, -1000000000);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%d %d", 1000000000, -1000000000);
	error += check_printf(22, "ft_printf(\"%d %d\", 1000000000, -1000000000) (ten digits)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%i", -2147483647);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%i", -2147483647);
	error += check_printf(23, "ft_printf(\"%i\", -2147483647)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%i %i", 9, 10);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%i %i", 9, 10);
	error += check_printf(24, "ft_printf(\"%i %i\", 9, 10) (one digit then two)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%u", 2147483648u);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%u", 2147483648u);
	error += check_printf(25, "ft_printf(\"%u\", 2147483648u) (INT_MAX + 1)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%u", -42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%u", -42);
	error += check_printf(26, "ft_printf(\"%u\", -42) (negative int wraps)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%u %u", 999999u, 1000000u);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%u %u", 999999u, 1000000u);
	error += check_printf(27, "ft_printf(\"%u %u\", 999999u, 1000000u)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%x", 0xdeadbeefu);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%x", 0xdeadbeefu);
	error += check_printf(28, "ft_printf(\"%x\", 0xdeadbeefu)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%x %x %x", 15, 16, 17);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%x %x %x", 15, 16, 17);
	error += check_printf(29, "ft_printf(\"%x %x %x\", 15, 16, 17) (one hex digit then two)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%X", 0xabcdef12u);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%X", 0xabcdef12u);
	error += check_printf(30, "ft_printf(\"%X\", 0xabcdef12u)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%x", INT_MIN);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%x", INT_MIN);
	error += check_printf(31, "ft_printf(\"%x\", INT_MIN) (80000000)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%x %X", -255, -255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%x %X", -255, -255);
	error += check_printf(32, "ft_printf(\"%x %X\", -255, -255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%p", (void *)0x1234abcd);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%p", (void *)0x1234abcd);
	error += check_printf(33, "ft_printf(\"%p\", (void *)0x1234abcd)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%p", (void *)16);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%p", (void *)16);
	error += check_printf(34, "ft_printf(\"%p\", (void *)16)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%p", (void *)0xdeadbeef);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%p", (void *)0xdeadbeef);
	error += check_printf(35, "ft_printf(\"%p\", (void *)0xdeadbeef)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%p %p", (void *)0, (void *)0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%p %p", (void *)0, (void *)0);
	error += check_printf(36, "ft_printf(\"%p %p\", (void *)0, (void *)0) (NULL twice)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%p", big);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%p", big);
	error += check_printf(37, "ft_printf(\"%p\", big) (a heap address)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%p%p", &local_var, &local_var);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%p%p", &local_var, &local_var);
	error += check_printf(38, "ft_printf(\"%p%p\", &local_var, &local_var) (no separator)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%%%d%%", 5);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%%%d%%", 5);
	error += check_printf(39, "ft_printf(\"%%%d%%\", 5) (percent, number, percent)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%d%%%d", 1, 2);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%d%%%d", 1, 2);
	error += check_printf(40, "ft_printf(\"%d%%%d\", 1, 2)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%%c");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%%c");
	error += check_printf(41, "ft_printf(\"%%c\") (a percent followed by a c is plain text)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%%s%s", "x");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%%s%s", "x");
	error += check_printf(42, "ft_printf(\"%%s%s\", \"x\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%%%%%%");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%%%%%%");
	error += check_printf(43, "ft_printf(\"%%%%%%\") (three escaped percents)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, pct_fmt);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, pct_fmt);
	error += check_printf(44, "ft_printf(\"%%\" repeated 1000 times)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, lit_fmt);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, lit_fmt);
	error += check_printf(45, "ft_printf(2600 characters of text, no conversion)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "x%dy%sz%cw", 1, "b", 'c');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "x%dy%sz%cw", 1, "b", 'c');
	error += check_printf(46, "ft_printf(\"x%dy%sz%cw\", 1, \"b\", 'c') (text glued to the conversions)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%d %s %c %u %x %X %i %d %s %c %u %x", 1, "s", 'c', 2u, 255, 255, -3, 4, "t", 'd', 5u, 4096);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%d %s %c %u %x %X %i %d %s %c %u %x", 1, "s", 'c', 2u, 255, 255, -3, 4, "t", 'd', 5u, 4096);
	error += check_printf(47, "ft_printf(\"%d %s %c %u %x %X %i %d %s %c %u %x\", 1, \"s\", 'c', 2u, 255, 255, -3, 4, \"t\", 'd', 5u, 4096) (twelve arguments in one call)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s|%c|%d|%u|%x|%X|%p|%i|%%", "s", 'c', -1, 1u, 10, 10, &local_var, 2);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s|%c|%d|%u|%x|%X|%p|%i|%%", "s", 'c', -1, 1u, 10, 10, &local_var, 2);
	error += check_printf(48, "ft_printf(\"%s|%c|%d|%u|%x|%X|%p|%i|%%\", \"s\", 'c', -1, 1u, 10, 10, &local_var, 2) (every conversion with separators)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	free(big);
	free(huge);
	free(pct_fmt);
	free(lit_fmt);
	return (error);
}
