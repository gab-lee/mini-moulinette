#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** %p with flags, expected as glibc prints it (the Linux output, also when
** mini runs on macOS): precision and '0' pad the digits after "0x", '+' and
** ' ' put a sign before "0x", and a NULL pointer is always "(nil)": never
** zero-padded, never signed, precision ignored. The expected text is given
** literally, so it does not depend on the host's printf.
*/

int main(void)
{
	int error;
	void *p;

	error = 0;
	p = (void *)0x2a;
	PF_CHECK(error, 1, "ft_printf(\"[%.5p]\", 0x2a) (precision pads after 0x)", real_printf,
		("[%.5p]", p), ("[%s]", "0x0002a"));
	PF_CHECK(error, 2, "ft_printf(\"[%.0p]\", 0x2a)", real_printf,
		("[%.0p]", p), ("[%s]", "0x2a"));
	PF_CHECK(error, 3, "ft_printf(\"[%010p]\", 0x2a) ('0' pads after 0x)", real_printf,
		("[%010p]", p), ("[%s]", "0x0000002a"));
	PF_CHECK(error, 4, "ft_printf(\"[%020p]\", 0x2a)", real_printf,
		("[%020p]", p), ("[%s]", "0x00000000000000002a"));
	PF_CHECK(error, 5, "ft_printf(\"[%+p]\", 0x2a) ('+' adds a sign)", real_printf,
		("[%+p]", p), ("[%s]", "+0x2a"));
	PF_CHECK(error, 6, "ft_printf(\"[% p]\", 0x2a) (' ' adds a blank)", real_printf,
		("[% p]", p), ("[%s]", " 0x2a"));
	PF_CHECK(error, 7, "ft_printf(\"[%+10p]\", 0x2a)", real_printf,
		("[%+10p]", p), ("[%s]", "     +0x2a"));
	PF_CHECK(error, 8, "ft_printf(\"[%+-10p]\", 0x2a)", real_printf,
		("[%+-10p]", p), ("[%s]", "+0x2a     "));
	PF_CHECK(error, 9, "ft_printf(\"[%+.5p]\", 0x2a)", real_printf,
		("[%+.5p]", p), ("[%s]", "+0x0002a"));
	PF_CHECK(error, 10, "ft_printf(\"[%010.5p]\", 0x2a) (precision wins over '0')", real_printf,
		("[%010.5p]", p), ("[%s]", "   0x0002a"));
	p = NULL;
	PF_CHECK(error, 11, "ft_printf(\"[%020p]\", NULL) ((nil) is never zero-padded)", real_printf,
		("[%020p]", p), ("[%s]", "               (nil)"));
	PF_CHECK(error, 12, "ft_printf(\"[%010p]\", NULL)", real_printf,
		("[%010p]", p), ("[%s]", "     (nil)"));
	PF_CHECK(error, 13, "ft_printf(\"[%.5p]\", NULL) (precision ignored)", real_printf,
		("[%.5p]", p), ("[%s]", "(nil)"));
	PF_CHECK(error, 14, "ft_printf(\"[%+p]\", NULL) (no sign)", real_printf,
		("[%+p]", p), ("[%s]", "(nil)"));
	PF_CHECK(error, 15, "ft_printf(\"[% p]\", NULL) (no blank)", real_printf,
		("[% p]", p), ("[%s]", "(nil)"));
	PF_CHECK(error, 16, "ft_printf(\"[%+10p]\", NULL)", real_printf,
		("[%+10p]", p), ("[%s]", "     (nil)"));
	PF_CHECK(error, 17, "ft_printf(\"[%-10p]\", NULL)", real_printf,
		("[%-10p]", p), ("[%s]", "(nil)     "));
	return (error);
}
