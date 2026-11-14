#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** %s with a NULL pointer, as glibc prints it: "(null)" when the precision
** is absent or at least 6, an empty string when it is below 6. The width
** still applies to what is printed.
*/

int main(void)
{
	int error;
	char *s;

	error = 0;
	s = NULL;
	PF_CHECK(error, 1, "ft_printf(\"[%.0s]\", NULL)", real_printf,
		("[%.0s]", s), ("[%s]", ""));
	PF_CHECK(error, 2, "ft_printf(\"[%.3s]\", NULL) (precision below 6: empty)", real_printf,
		("[%.3s]", s), ("[%s]", ""));
	PF_CHECK(error, 3, "ft_printf(\"[%.5s]\", NULL)", real_printf,
		("[%.5s]", s), ("[%s]", ""));
	PF_CHECK(error, 4, "ft_printf(\"[%.6s]\", NULL) (precision 6: whole text)", real_printf,
		("[%.6s]", s), ("[%s]", "(null)"));
	PF_CHECK(error, 5, "ft_printf(\"[%.100s]\", NULL)", real_printf,
		("[%.100s]", s), ("[%s]", "(null)"));
	PF_CHECK(error, 6, "ft_printf(\"[%8.3s]\", NULL) (width over an empty string)", real_printf,
		("[%8.3s]", s), ("[%s]", "        "));
	PF_CHECK(error, 7, "ft_printf(\"[%-8.3s]\", NULL)", real_printf,
		("[%-8.3s]", s), ("[%s]", "        "));
	PF_CHECK(error, 8, "ft_printf(\"[%8.6s]\", NULL)", real_printf,
		("[%8.6s]", s), ("[%s]", "  (null)"));
	PF_CHECK(error, 9, "ft_printf(\"[%-8.6s]\", NULL)", real_printf,
		("[%-8.6s]", s), ("[%s]", "(null)  "));
	PF_CHECK(error, 10, "ft_printf(\"[%.s]\", NULL) ('.' alone is precision 0)", real_printf,
		("[%.s]", s), ("[%s]", ""));
	return (error);
}
