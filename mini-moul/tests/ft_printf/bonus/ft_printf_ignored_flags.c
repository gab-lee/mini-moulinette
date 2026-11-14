#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** Flags that do not apply to a conversion are ignored, and the width still
** applies: '#' on d i u s c p, '0' on s and c (padded with spaces), and a
** width, '0' or '-' on %% (a single '%' is printed).
*/

int main(void)
{
	int error;
	void *p;

	error = 0;
	p = (void *)0x2a;
	PF_CHECK(error, 1, "ft_printf(\"[%#d]\", 42)", real_printf,
		("[%#d]", 42), ("[%s]", "42"));
	PF_CHECK(error, 2, "ft_printf(\"[%#i]\", -7)", real_printf,
		("[%#i]", -7), ("[%s]", "-7"));
	PF_CHECK(error, 3, "ft_printf(\"[%#u]\", 3u)", real_printf,
		("[%#u]", 3u), ("[%s]", "3"));
	PF_CHECK(error, 4, "ft_printf(\"[%#s]\", \"x\")", real_printf,
		("[%#s]", "x"), ("[%s]", "x"));
	PF_CHECK(error, 5, "ft_printf(\"[%#c]\", 'c')", real_printf,
		("[%#c]", 'c'), ("[%s]", "c"));
	PF_CHECK(error, 6, "ft_printf(\"[%#p]\", 0x2a)", real_printf,
		("[%#p]", p), ("[%s]", "0x2a"));
	PF_CHECK(error, 7, "ft_printf(\"[%#10d]\", 42) (width still applies)", real_printf,
		("[%#10d]", 42), ("[%s]", "        42"));
	PF_CHECK(error, 8, "ft_printf(\"[%#5u]\", 3u)", real_printf,
		("[%#5u]", 3u), ("[%s]", "    3"));
	PF_CHECK(error, 9, "ft_printf(\"[%#5s]\", \"ab\")", real_printf,
		("[%#5s]", "ab"), ("[%s]", "   ab"));
	PF_CHECK(error, 10, "ft_printf(\"[%#-5c]\", 'a')", real_printf,
		("[%#-5c]", 'a'), ("[%s]", "a    "));
	PF_CHECK(error, 11, "ft_printf(\"[%#8p]\", 0x2a)", real_printf,
		("[%#8p]", p), ("[%s]", "    0x2a"));
	PF_CHECK(error, 12, "ft_printf(\"[%#08d]\", 42)", real_printf,
		("[%#08d]", 42), ("[%s]", "00000042"));
	PF_CHECK(error, 13, "ft_printf(\"[%05s]\", \"ab\") ('0' pads s with spaces)", real_printf,
		("[%05s]", "ab"), ("[%s]", "   ab"));
	PF_CHECK(error, 14, "ft_printf(\"[%05c]\", 'a') ('0' pads c with spaces)", real_printf,
		("[%05c]", 'a'), ("[%s]", "    a"));
	PF_CHECK(error, 15, "ft_printf(\"[%-05c]\", 'a')", real_printf,
		("[%-05c]", 'a'), ("[%s]", "a    "));
	PF_CHECK(error, 16, "ft_printf(\"[%5%]\") (width ignored)", real_printf,
		("[%5%]"), ("[%s]", "%"));
	PF_CHECK(error, 17, "ft_printf(\"[%-5%]\")", real_printf,
		("[%-5%]"), ("[%s]", "%"));
	PF_CHECK(error, 18, "ft_printf(\"[%05%]\")", real_printf,
		("[%05%]"), ("[%s]", "%"));
	PF_CHECK(error, 19, "ft_printf(\"[%5%|%d]\", 1) (the next argument is not consumed)", real_printf,
		("[%5%|%d]", 1), ("[%s]", "%|1"));
	return (error);
}
