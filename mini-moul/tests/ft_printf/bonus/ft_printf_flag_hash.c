#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/* '#' flag: alternate form. For x/X, prefixes a nonzero result with
** 0x/0X; a result of exactly 0 gets no prefix. */

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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#x]", 255);
	error += check_printf(1, "ft_printf(\"[%#x]\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#X]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#X]", 255);
	error += check_printf(2, "ft_printf(\"[%#X]\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#x]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#x]", 0);
	error += check_printf(3, "ft_printf(\"[%#x]\", 0) (zero gets no prefix)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#12x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#12x]", 255);
	error += check_printf(4, "ft_printf(\"[%#12x]\", 255) (with width)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#012x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#012x]", 255);
	error += check_printf(5, "ft_printf(\"[%#012x]\", 255) (with width and zero-pad)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
