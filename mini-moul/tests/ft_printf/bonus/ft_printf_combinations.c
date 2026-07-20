#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** The subject asks for "any combination" of flags, width and precision
** across all conversions. Each flag/width/precision is exercised alone
** elsewhere in this part; this file stacks several together per case,
** the way real format strings actually look.
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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-+10d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-+10d]", 42);
	error += check_printf(1, "ft_printf(\"[%-+10d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%0+10.5d]", 42);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%0+10.5d]", 42);
	error += check_printf(2, "ft_printf(\"[%0+10.5d]\", 42)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-#12x]", 255);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-#12x]", 255);
	error += check_printf(3, "ft_printf(\"[%-#12x]\", 255)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%#010.4X]", 15);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%#010.4X]", 15);
	error += check_printf(4, "ft_printf(\"[%#010.4X]\", 15)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%-15.3s]", "hello world");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%-15.3s]", "hello world");
	error += check_printf(5, "ft_printf(\"[%-15.3s]\", \"hello world\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[% -10u]", 42u);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[% -10u]", 42u);
	error += check_printf(6, "ft_printf(\"[% -10u]\", 42u)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine,
		"[%+05d] [%-8.2s] [%#x] [%5c]", -3, "abcdef", 10, 'Z');
	PF_RUN(real_printf, ret_ref, out_ref, len_ref,
		"[%+05d] [%-8.2s] [%#x] [%5c]", -3, "abcdef", 10, 'Z');
	error += check_printf(7, "ft_printf(\"[%+05d] [%-8.2s] [%#x] [%5c]\", ...) (several conversions, each with its own flags)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
