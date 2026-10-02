#include <stdio.h>
#include <stdlib.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/libc_compare.h"

typedef struct s_test
{
	char *desc;
	char *s;
} t_test;

int main(void)
{
	t_test tests[] = {
	    {.desc = "ft_atoi(\"0\")", .s = "0"},
	    {.desc = "ft_atoi(\"42\")", .s = "42"},
	    {.desc = "ft_atoi(\"-42\")", .s = "-42"},
	    {.desc = "ft_atoi(\"+42\")", .s = "+42"},
	    {.desc = "ft_atoi(\"   42\") (leading spaces)", .s = "   42"},
	    {.desc = "ft_atoi(\" \\t\\n\\v\\f\\r 42\") (all whitespace)", .s = " \t\n\v\f\r 42"},
	    {.desc = "ft_atoi(\"42abc\") (trailing garbage)", .s = "42abc"},
	    {.desc = "ft_atoi(\"abc\") (no digits)", .s = "abc"},
	    {.desc = "ft_atoi(\"\") (empty string)", .s = ""},
	    {.desc = "ft_atoi(\"-+42\") (double sign)", .s = "-+42"},
	    {.desc = "ft_atoi(\"  +-42\") (double sign)", .s = "  +-42"},
	    {.desc = "ft_atoi(\"0000042\") (leading zeros)", .s = "0000042"},
	    {.desc = "ft_atoi(\"-0042\")", .s = "-0042"},
	    {.desc = "ft_atoi(\"2147483647\") (INT_MAX)", .s = "2147483647"},
	    {.desc = "ft_atoi(\"-2147483648\") (INT_MIN)", .s = "-2147483648"},
	    {.desc = "ft_atoi(\"   \") (only whitespace)", .s = "   "},
	    {.desc = "ft_atoi(\"\\t\\n\\v\\f\\ra1\") (letter before digits)", .s = "\t\n\v\f\ra1"},
	    {.desc = "ft_atoi(\"\\t\\n\\v\\f\\r--1\") (double minus)", .s = "\t\n\v\f\r--1"},
	    {.desc = "ft_atoi(\"\\t\\n\\v\\f\\r++1\") (double plus)", .s = "\t\n\v\f\r++1"},
	    {.desc = "ft_atoi(\"\\t\\n\\v\\f\\r+42lyon\")", .s = "\t\n\v\f\r+42lyon"},
	    {.desc = "ft_atoi(\"\\t\\n\\v\\f\\r+101\")", .s = "\t\n\v\f\r+101"},
	    {.desc = "ft_atoi(\"+-42\") (double sign)", .s = "+-42"},
	    {.desc = "ft_atoi(\"+\\t\\n\\v\\f\\r42\") (whitespace after sign)", .s = "+\t\n\v\f\r42"},
	    {.desc = "ft_atoi(\"-\\t\\n\\v\\f\\r42\") (whitespace after sign)", .s = "-\t\n\v\f\r42"},
	    {.desc = "ft_atoi(\"1\\t\\n\\v\\f\\r42\") (whitespace stops parsing)", .s = "1\t\n\v\f\r42"},
	    {.desc = "ft_atoi(\"-1\\t\\n\\v\\f\\r42\") (whitespace stops parsing)", .s = "-1\t\n\v\f\r42"},
	};
	int count = sizeof(tests) / sizeof(tests[0]);
	int error = 0;
	int i;

	for (i = 0; i < count; i++)
		error += check_long(i + 1, tests[i].desc,
			(long)ft_atoi(tests[i].s), (long)atoi(tests[i].s));

	return (error);
}
