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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%i", 0);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%i", 0);
	error += check_printf(1, "ft_printf(\"%i\", 0)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%i", 123);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%i", 123);
	error += check_printf(2, "ft_printf(\"%i\", 123)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%i", -123);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%i", -123);
	error += check_printf(3, "ft_printf(\"%i\", -123)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%i", INT_MIN);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%i", INT_MIN);
	error += check_printf(4, "ft_printf(\"%i\", INT_MIN) (overflow trap)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "score: %i pts", 7);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "score: %i pts", 7);
	error += check_printf(5, "ft_printf(\"score: %i pts\", 7)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
