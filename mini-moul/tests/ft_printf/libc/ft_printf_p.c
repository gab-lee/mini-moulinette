#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/printf_compare.h"

/*
** %p prints the address itself, which differs from run to run (ASLR) and
** even from call to call for stack locals. Every case below passes the
** SAME pointer value to both ft_printf and printf so the comparison is
** apples-to-apples regardless of where that value actually points.
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
	int local_var;
	char buf[8];
	void *ptr;

	error = 0;

	ptr = &local_var;
	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%p", ptr);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%p", ptr);
	error += check_printf(1, "ft_printf(\"%p\", &local_var)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	ptr = buf;
	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%p", ptr);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%p", ptr);
	error += check_printf(2, "ft_printf(\"%p\", buf)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	ptr = NULL;
	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%p", ptr);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%p", ptr);
	error += check_printf(3, "ft_printf(\"%p\", NULL)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	ptr = &local_var;
	PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "ptr=%p end", ptr);
	PF_RUN(real_printf, ret_ref, out_ref, len_ref, "ptr=%p end", ptr);
	error += check_printf(4, "ft_printf(\"ptr=%p end\", &local_var)",
		ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
	free(out_mine);
	free(out_ref);

	return (error);
}
