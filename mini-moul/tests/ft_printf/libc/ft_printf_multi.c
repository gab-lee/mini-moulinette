#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** Each conversion is tested in isolation elsewhere in this part. This file
** combines several in one format string, the way real code actually uses
** printf, to catch parsing bugs isolated single-conversion cases wouldn't
** (state carried across conversions, literal text interleaving, etc.).
*/

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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine,
		"Name: %s, Age: %d, Grade: %c\n", "Ada", 36, 'A');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref,
		"Name: %s, Age: %d, Grade: %c\n", "Ada", 36, 'A');
	error += check_printf(1, "ft_printf(\"Name: %s, Age: %d, Grade: %c\\n\", ...)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine,
		"%d in hex is 0x%x, in upper hex is 0x%X, unsigned is %u",
		-1, -1, -1, -1);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref,
		"%d in hex is 0x%x, in upper hex is 0x%X, unsigned is %u",
		-1, -1, -1, -1);
	error += check_printf(2, "ft_printf(\"%d in hex is 0x%x...\", -1, -1, -1, -1)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "no conversions here at all");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "no conversions here at all");
	error += check_printf(3, "ft_printf(\"no conversions here at all\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "");
	error += check_printf(4, "ft_printf(\"\") (empty format)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine,
		"%c%s%d%i%u%x%X%%", 'a', "bc", 1, -1, 2, 255, 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref,
		"%c%s%d%i%u%x%X%%", 'a', "bc", 1, -1, 2, 255, 255);
	error += check_printf(5, "ft_printf(\"%c%s%d%i%u%x%X%%\", ...) (every conversion back-to-back)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
