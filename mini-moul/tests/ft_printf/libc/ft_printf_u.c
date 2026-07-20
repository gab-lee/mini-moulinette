#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%u", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%u", 0);
	error += check_printf(1, "ft_printf(\"%u\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%u", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%u", 42);
	error += check_printf(2, "ft_printf(\"%u\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%u", -1);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%u", -1);
	error += check_printf(3, "ft_printf(\"%u\", -1) (wraps to UINT_MAX)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%u", UINT_MAX);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%u", UINT_MAX);
	error += check_printf(4, "ft_printf(\"%u\", UINT_MAX)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "count=%u.", 999u);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "count=%u.", 999u);
	error += check_printf(5, "ft_printf(\"count=%u.\", 999u)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
