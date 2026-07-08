#include <stdio.h>
#include <string.h>
#include "../../../../ft_strlen.c"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

int main(void)
{
	int error = 0;
	char big[1001];
	int i;

	for (i = 0; i < 1000; i++)
		big[i] = 'a' + (i % 26);
	big[1000] = '\0';

	error += check_long(1, "ft_strlen(\"\") (empty string)",
		(long)ft_strlen(""), (long)strlen(""));
	error += check_long(2, "ft_strlen(\"a\")",
		(long)ft_strlen("a"), (long)strlen("a"));
	error += check_long(3, "ft_strlen(\"hello, world\")",
		(long)ft_strlen("hello, world"), (long)strlen("hello, world"));
	error += check_long(4, "ft_strlen(\" \\t spaces and tabs \\n\")",
		(long)ft_strlen(" \t spaces and tabs \n"), (long)strlen(" \t spaces and tabs \n"));
	error += check_long(5, "ft_strlen(\"42\\0hidden\") stops at first NUL",
		(long)ft_strlen("42\0hidden"), (long)strlen("42\0hidden"));
	error += check_long(6, "ft_strlen(1000-char string)",
		(long)ft_strlen(big), (long)strlen(big));

	return (error);
}
