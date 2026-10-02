#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/* '+' flag: forces an explicit sign on signed conversions. Positive
** values get a leading '+'; negative values are unaffected (still '-').
** Doesn't apply to unsigned conversions (u/x/X). */

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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+d]", 42);
	error += check_printf(1, "ft_printf(\"[%+d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+d]", -42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+d]", -42);
	error += check_printf(2, "ft_printf(\"[%+d]\", -42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+d]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+d]", 0);
	error += check_printf(3, "ft_printf(\"[%+d]\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+i]", 7);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+i]", 7);
	error += check_printf(4, "ft_printf(\"[%+i]\", 7)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+08d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+08d]", 42);
	error += check_printf(5, "ft_printf(\"[%+08d]\", 42) (with width and zero-pad)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%+u]", 42u);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%+u]", 42u);
	error += check_printf(6, "ft_printf(\"[%+u]\", 42u) ('+' has no effect on %%u)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
