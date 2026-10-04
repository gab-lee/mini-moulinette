#include "../try.h"

int	main(int argc, char **argv)
{
	t_list	*lst;
	int		calls;

	try_args(argc, 0, 255, "lstclear [node ...]");
	lst = try_list(argc - 1, argv + 1);
	printf(TRY_BOLD "ft_lstclear" DEFAULT "(&lst, del) with lst ");
	try_put_list(lst);
	ft_lstclear(&lst, try_del);
	calls = *try_del_count();
	try_row("ft");
	printf("del called %d time%s, lst is %s\n", calls, calls == 1 ? "" : "s",
		lst ? "not NULL" : "NULL");
	return (try_expect(calls == argc - 1 && !lst,
			"one del call per node, lst set to NULL"));
}
