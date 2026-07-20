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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", 'A');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", 'A');
	error += check_printf(1, "ft_printf(\"%c\", 'A')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", '5');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", '5');
	error += check_printf(2, "ft_printf(\"%c\", '5')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", ' ');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", ' ');
	error += check_printf(3, "ft_printf(\"%c\", ' ')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "char: %c!", 'z');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "char: %c!", 'z');
	error += check_printf(4, "ft_printf(\"char: %c!\", 'z')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c%c%c", 'x', 'y', 'z');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c%c%c", 'x', 'y', 'z');
	error += check_printf(5, "ft_printf(\"%c%c%c\", 'x', 'y', 'z')",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", 200);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", 200);
	error += check_printf(6, "ft_printf(\"%c\", 200) (extended byte value)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
