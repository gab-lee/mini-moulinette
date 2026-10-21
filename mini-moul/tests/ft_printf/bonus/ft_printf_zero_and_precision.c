#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** Precision edge cases: a missing or zero precision with the value 0 prints no
** digits (but '+', ' ' and the width still apply), a precision pads with
** zeros after the sign, and '0' is ignored once a precision is given.
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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.d]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.d]", 0);
	error += check_printf(1, "ft_printf(\"[%.d]\", 0) (a missing precision means 0: no digits)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.d]", 42);
	error += check_printf(2, "ft_printf(\"[%.d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%5.d]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%5.d]", 0);
	error += check_printf(3, "ft_printf(\"[%5.d]\", 0) (width still applies)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.0d]", 1);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.0d]", 1);
	error += check_printf(4, "ft_printf(\"[%.0d]\", 1) (precision 0 only hides the value 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.1d]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.1d]", 0);
	error += check_printf(5, "ft_printf(\"[%.1d]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+.0d]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+.0d]", 0);
	error += check_printf(6, "ft_printf(\"[%+.0d]\", 0) (the sign stays)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[% .0d]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[% .0d]", 0);
	error += check_printf(7, "ft_printf(\"[% .0d]\", 0) (the space stays)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.0i]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.0i]", 0);
	error += check_printf(8, "ft_printf(\"[%.0i]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-5.0d]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-5.0d]", 0);
	error += check_printf(9, "ft_printf(\"[%-5.0d]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.0u]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.0u]", 0);
	error += check_printf(10, "ft_printf(\"[%.0u]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%5.0u]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%5.0u]", 0);
	error += check_printf(11, "ft_printf(\"[%5.0u]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.0X]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.0X]", 0);
	error += check_printf(12, "ft_printf(\"[%.0X]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#.0x]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#.0x]", 0);
	error += check_printf(13, "ft_printf(\"[%#.0x]\", 0) (no prefix for 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#.0X]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#.0X]", 0);
	error += check_printf(14, "ft_printf(\"[%#.0X]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#5.3x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#5.3x]", 255);
	error += check_printf(15, "ft_printf(\"[%#5.3x]\", 255) (prefix, precision and width together)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#.5X]", 0xabc);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#.5X]", 0xabc);
	error += check_printf(16, "ft_printf(\"[%#.5X]\", 0xabc)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.3d]", -5);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.3d]", -5);
	error += check_printf(17, "ft_printf(\"[%.3d]\", -5) (zeros after the sign)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%8.3d]", -5);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%8.3d]", -5);
	error += check_printf(18, "ft_printf(\"[%8.3d]\", -5)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-8.3d]", -5);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-8.3d]", -5);
	error += check_printf(19, "ft_printf(\"[%-8.3d]\", -5)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%08.3d]", -5);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%08.3d]", -5);
	error += check_printf(20, "ft_printf(\"[%08.3d]\", -5) ('0' ignored with a precision)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+8.3d]", 5);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+8.3d]", 5);
	error += check_printf(21, "ft_printf(\"[%+8.3d]\", 5)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[% 8.3d]", 5);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[% 8.3d]", 5);
	error += check_printf(22, "ft_printf(\"[% 8.3d]\", 5)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.3u]", 7u);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.3u]", 7u);
	error += check_printf(23, "ft_printf(\"[%.3u]\", 7u)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%10.4x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%10.4x]", 255);
	error += check_printf(24, "ft_printf(\"[%10.4x]\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
