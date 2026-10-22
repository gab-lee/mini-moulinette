#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** Large widths and precisions (bigger than any internal buffer a student
** might have used), NUL characters and NULL pointers under a width, and
** strings shorter than, equal to and longer than the width or precision.
*/

int main(void)
{
	int local_var;
	int error;
	int ret_mine;
	int ret_ref;
	char *out_mine;
	char *out_ref;
	long len_mine;
	long len_ref;

	error = 0;
	local_var = 0;

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%100d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%100d]", 42);
	error += check_printf(1, "ft_printf(\"[%100d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-100d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-100d]", 42);
	error += check_printf(2, "ft_printf(\"[%-100d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.100d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.100d]", 42);
	error += check_printf(3, "ft_printf(\"[%.100d]\", 42) (precision wider than a buffer)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%0100d]", -42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%0100d]", -42);
	error += check_printf(4, "ft_printf(\"[%0100d]\", -42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%200.150d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%200.150d]", 42);
	error += check_printf(5, "ft_printf(\"[%200.150d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%5000d]", INT_MIN);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%5000d]", INT_MIN);
	error += check_printf(6, "ft_printf(\"[%5000d]\", INT_MIN)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%1000s]", "x");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%1000s]", "x");
	error += check_printf(7, "ft_printf(\"[%1000s]\", \"x\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-1000s]", "x");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-1000s]", "x");
	error += check_printf(8, "ft_printf(\"[%-1000s]\", \"x\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.1000s]", "short");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.1000s]", "short");
	error += check_printf(9, "ft_printf(\"[%.1000s]\", \"short\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%100c]", 'a');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%100c]", 'a');
	error += check_printf(10, "ft_printf(\"[%100c]\", 'a')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%5c]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%5c]", 0);
	error += check_printf(11, "ft_printf(\"[%5c]\", 0) (a NUL with a width)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-5c]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-5c]", 0);
	error += check_printf(12, "ft_printf(\"[%-5c]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%100p]", &local_var);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%100p]", &local_var);
	error += check_printf(13, "ft_printf(\"[%100p]\", &local_var)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%10p]", NULL);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%10p]", NULL);
	error += check_printf(14, "ft_printf(\"[%10p]\", NULL)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-10p]", NULL);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-10p]", NULL);
	error += check_printf(15, "ft_printf(\"[%-10p]\", NULL)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%5s]", "");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%5s]", "");
	error += check_printf(16, "ft_printf(\"[%5s]\", \"\") (empty string with a width)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-5s]", "");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-5s]", "");
	error += check_printf(17, "ft_printf(\"[%-5s]\", \"\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%6s]", NULL);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%6s]", NULL);
	error += check_printf(18, "ft_printf(\"[%6s]\", NULL) (width equal to the length of (null))",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%5s]", NULL);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%5s]", NULL);
	error += check_printf(19, "ft_printf(\"[%5s]\", NULL) (width smaller than (null))",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-10s]", NULL);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-10s]", NULL);
	error += check_printf(20, "ft_printf(\"[%-10s]\", NULL)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%10.3s]", "hello");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%10.3s]", "hello");
	error += check_printf(21, "ft_printf(\"[%10.3s]\", \"hello\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%10.0s]", "hello");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%10.0s]", "hello");
	error += check_printf(22, "ft_printf(\"[%10.0s]\", \"hello\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-10.10s]", "hi");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-10.10s]", "hi");
	error += check_printf(23, "ft_printf(\"[%-10.10s]\", \"hi\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%3.5s]", "hello");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%3.5s]", "hello");
	error += check_printf(24, "ft_printf(\"[%3.5s]\", \"hello\") (width smaller than the precision)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.3s|%.0s|%.1s]", "abc", "def", "ghi");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.3s|%.0s|%.1s]", "abc", "def", "ghi");
	error += check_printf(25, "ft_printf(\"[%.3s|%.0s|%.1s]\", \"abc\", \"def\", \"ghi\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%5.3s]", "hello world");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%5.3s]", "hello world");
	error += check_printf(26, "ft_printf(\"[%5.3s]\", \"hello world\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
