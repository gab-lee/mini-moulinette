#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** Flags may come in any order and be repeated; '-' overrides '0', '+'
** overrides ' ', and a precision overrides '0'. This file tries the orders and
** the repeats that implementations that only parse one fixed order miss.
*/

int main(void)
{
	int error;
	int ret_mine;
	int ret_ref;
	char *out_mine;
	char *out_ref;
	long len_mine;
	long len_ref;

	error = 0;

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+-8d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+-8d]", 42);
	error += check_printf(1, "ft_printf(\"[%+-8d]\", 42) ('-' before and after '+')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-+8d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-+8d]", 42);
	error += check_printf(2, "ft_printf(\"[%-+8d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%0-8d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%0-8d]", 42);
	error += check_printf(3, "ft_printf(\"[%0-8d]\", 42) ('-' overrides '0' in either order)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-08d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-08d]", 42);
	error += check_printf(4, "ft_printf(\"[%-08d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%--8d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%--8d]", 42);
	error += check_printf(5, "ft_printf(\"[%--8d]\", 42) (a repeated flag)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%00008d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%00008d]", 42);
	error += check_printf(6, "ft_printf(\"[%00008d]\", 42) (a run of zeros is one flag then the width)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%++d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%++d]", 42);
	error += check_printf(7, "ft_printf(\"[%++d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%  d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%  d]", 42);
	error += check_printf(8, "ft_printf(\"[%  d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-#8x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-#8x]", 255);
	error += check_printf(9, "ft_printf(\"[%-#8x]\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#-8x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#-8x]", 255);
	error += check_printf(10, "ft_printf(\"[%#-8x]\", 255) (same flags, other order)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#08x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#08x]", 255);
	error += check_printf(11, "ft_printf(\"[%#08x]\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%0#8x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%0#8x]", 255);
	error += check_printf(12, "ft_printf(\"[%0#8x]\", 255) (same flags, other order)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#-08x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#-08x]", 255);
	error += check_printf(13, "ft_printf(\"[%#-08x]\", 255) ('-' wins over '0')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[% +05d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[% +05d]", 42);
	error += check_printf(14, "ft_printf(\"[% +05d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+ 5d]", -42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+ 5d]", -42);
	error += check_printf(15, "ft_printf(\"[%+ 5d]\", -42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-+ 8d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-+ 8d]", 42);
	error += check_printf(16, "ft_printf(\"[%-+ 8d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%0 8d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%0 8d]", 42);
	error += check_printf(17, "ft_printf(\"[%0 8d]\", 42) ('0' and ' ' together)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+x]", 255);
	error += check_printf(18, "ft_printf(\"[%+x]\", 255) ('+' has no effect on %x)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[% X]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[% X]", 255);
	error += check_printf(19, "ft_printf(\"[% X]\", 255) (' ' has no effect on %X)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+5u]", 7u);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+5u]", 7u);
	error += check_printf(20, "ft_printf(\"[%+5u]\", 7u)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-+10.5d]", -42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-+10.5d]", -42);
	error += check_printf(21, "ft_printf(\"[%-+10.5d]\", -42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+.3d]", 5);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+.3d]", 5);
	error += check_printf(22, "ft_printf(\"[%+.3d]\", 5)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
