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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%X", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%X", 0);
	error += check_printf(1, "ft_printf(\"%X\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%X", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%X", 255);
	error += check_printf(2, "ft_printf(\"%X\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%X", -1);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%X", -1);
	error += check_printf(3, "ft_printf(\"%X\", -1) (wraps to FFFFFFFF)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%X", UINT_MAX);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%X", UINT_MAX);
	error += check_printf(4, "ft_printf(\"%X\", UINT_MAX)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "ADDR: 0x%X", 4096);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "ADDR: 0x%X", 4096);
	error += check_printf(5, "ft_printf(\"ADDR: 0x%X\", 4096)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
