#include "../try.h"

int	main(int argc, char **argv)
{
	t_list	*lst;
	t_list	*last;
	t_list	*mine;

	try_args(argc, 0, 255, "lstlast [node ...]");
	lst = try_list(argc - 1, argv + 1);
	printf(TRY_BOLD "ft_lstlast" DEFAULT "(lst) with lst ");
	try_put_list(lst);
	last = lst;
	while (last && last->next)
		last = last->next;
	mine = ft_lstlast(lst);
	try_row("ft");
	if (mine)
	{
		printf("node ");
		try_put_str(mine->content);
		printf("\n");
	}
	else
		printf("NULL\n");
	try_free_list(lst);
	return (try_verdict(mine == last, "the last node"));
}
