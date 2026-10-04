#include "../try.h"

/* Deletes the first node; the rest of the list is freed by the driver. */
int	main(int argc, char **argv)
{
	t_list	*lst;
	t_list	*rest;
	int		calls;

	try_args(argc, 0, 255, "lstdelone [node ...]   (deletes the first node)");
	lst = try_list(argc - 1, argv + 1);
	printf(TRY_BOLD "ft_lstdelone" DEFAULT "(first node, del) with lst ");
	try_put_list(lst);
	rest = lst ? lst->next : NULL;
	ft_lstdelone(lst, try_del);
	calls = *try_del_count();
	try_row("ft");
	printf("del called %d time%s\n", calls, calls == 1 ? "" : "s");
	try_free_list(rest);
	return (try_expect(calls == (lst != NULL),
			lst ? "one del call" : "no del call for a NULL node"));
}
