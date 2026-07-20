#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/* '.' precision: minimum digits for integers (zero-padded, '0' flag then
** ignored), maximum characters printed for strings. */

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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.5d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.5d]", 42);
	error += check_printf(1, "ft_printf(\"[%.5d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.0d]", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.0d]", 0);
	error += check_printf(2, "ft_printf(\"[%.0d]\", 0) (precision 0, value 0 prints nothing)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.3s]", "hello");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.3s]", "hello");
	error += check_printf(3, "ft_printf(\"[%.3s]\", \"hello\") (truncated)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.10s]", "hi");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.10s]", "hi");
	error += check_printf(4, "ft_printf(\"[%.10s]\", \"hi\") (precision longer than string)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.8x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.8x]", 255);
	error += check_printf(5, "ft_printf(\"[%.8x]\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%010.5d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%010.5d]", 42);
	error += check_printf(6, "ft_printf(\"[%010.5d]\", 42) ('0' ignored when precision is set)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%.5d]", -42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%.5d]", -42);
	error += check_printf(7, "ft_printf(\"[%.5d]\", -42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
