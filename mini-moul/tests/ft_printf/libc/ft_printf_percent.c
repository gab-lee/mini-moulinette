#include <stdio.h>
#include <stdlib.h>
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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%%");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%%");
	error += check_printf(1, "ft_printf(\"%%\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "100%% done");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "100%% done");
	error += check_printf(2, "ft_printf(\"100%% done\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%%%%");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%%%%");
	error += check_printf(3, "ft_printf(\"%%%%\") (two escaped percents)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%d%%", 50);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%d%%", 50);
	error += check_printf(4, "ft_printf(\"%d%%\", 50)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
