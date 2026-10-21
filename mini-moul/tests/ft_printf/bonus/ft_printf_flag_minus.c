#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/* '-' flag: left-justify within the given field width. */

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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-10d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-10d]", 42);
	error += check_printf(1, "ft_printf(\"[%-10d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-10s]", "hi");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-10s]", "hi");
	error += check_printf(2, "ft_printf(\"[%-10s]\", \"hi\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-5c]", 'x');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-5c]", 'x');
	error += check_printf(3, "ft_printf(\"[%-5c]\", 'x')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-8x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-8x]", 255);
	error += check_printf(4, "ft_printf(\"[%-8x]\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-4d]", 123456);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-4d]", 123456);
	error += check_printf(5, "ft_printf(\"[%-4d]\", 123456) (content wider than width)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-6d]", -42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-6d]", -42);
	error += check_printf(6, "ft_printf(\"[%-6d]\", -42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
