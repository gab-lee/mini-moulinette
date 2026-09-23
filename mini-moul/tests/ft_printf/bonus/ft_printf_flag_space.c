#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/* ' ' (space) flag: a positive signed result gets a leading space where
** '+' would put a sign. If both ' ' and '+' are given, '+' wins. */

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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[% d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[% d]", 42);
	error += check_printf(1, "ft_printf(\"[% d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[% d]", -42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[% d]", -42);
	error += check_printf(2, "ft_printf(\"[% d]\", -42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[% i]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[% i]", 0);
	error += check_printf(3, "ft_printf(\"[% i]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[% 08d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[% 08d]", 42);
	error += check_printf(4, "ft_printf(\"[% 08d]\", 42) (with width and zero-pad)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+ d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+ d]", 42);
	error += check_printf(5, "ft_printf(\"[%+ d]\", 42) ('+' wins over ' ')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
