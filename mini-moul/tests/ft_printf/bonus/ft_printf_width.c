#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/* Minimum field width, no other flag: pads with spaces (right-justified
** by default). */

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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%10d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%10d]", 42);
	error += check_printf(1, "ft_printf(\"[%10d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%6s]", "hi");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%6s]", "hi");
	error += check_printf(2, "ft_printf(\"[%6s]\", \"hi\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%5c]", 'x');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%5c]", 'x');
	error += check_printf(3, "ft_printf(\"[%5c]\", 'x')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%8x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%8x]", 255);
	error += check_printf(4, "ft_printf(\"[%8x]\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%3d]", 123456);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%3d]", 123456);
	error += check_printf(5, "ft_printf(\"[%3d]\", 123456) (content wider than width)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%4%]");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%4%]");
	error += check_printf(6, "ft_printf(\"[%4%]\") (width with the %% conversion)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
