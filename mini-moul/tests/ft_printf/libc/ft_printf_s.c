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

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s", "hello");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s", "hello");
	error += check_printf(1, "ft_printf(\"%s\", \"hello\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s", "");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s", "");
	error += check_printf(2, "ft_printf(\"%s\", \"\") (empty string)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "value: %s.", "world");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "value: %s.", "world");
	error += check_printf(3, "ft_printf(\"value: %s.\", \"world\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s and %s", "foo", "bar");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s and %s", "foo", "bar");
	error += check_printf(4, "ft_printf(\"%s and %s\", \"foo\", \"bar\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%s", "Hello, World! 123");
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%s", "Hello, World! 123");
	error += check_printf(5, "ft_printf(\"%s\", \"Hello, World! 123\")",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "[%s]", (char *)NULL);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "[%s]", (char *)NULL);
	error += check_printf(6, "ft_printf(\"[%s]\", NULL)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
