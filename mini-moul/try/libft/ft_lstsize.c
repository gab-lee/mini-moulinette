#include "../try.h"

int	main(int argc, char **argv)
{
	t_list	*lst;
	long	mine;

	try_args(argc, 0, 255, "lstsize [node ...]");
	lst = try_list(argc - 1, argv + 1);
	printf(TRY_BOLD "ft_lstsize" DEFAULT "(lst) with lst ");
	try_put_list(lst);
	mine = (long)ft_lstsize(lst);
	try_row("ft");
	printf("%ld\n", mine);
	try_free_list(lst);
	return (try_verdict(mine == argc - 1, "the node count"));
}
