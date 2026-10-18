#include "../try.h"

int	main(int argc, char **argv)
{
	t_list	*lst;
	t_list	*head;
	t_list	*new;
	t_list	*last;
	int		same;

	try_args(argc, 1, 255, "lstadd_back <new> [node ...]");
	new = try_list(1, argv + 1);
	lst = try_list(argc - 2, argv + 2);
	head = lst;
	printf(TRY_BOLD "ft_lstadd_back" DEFAULT "(&lst, new node ");
	try_put_str(new->content);
	printf(") with lst ");
	try_put_list(lst);
	ft_lstadd_back(&lst, new);
	try_row("ft");
	try_put_list(lst);
	last = lst;
	while (last && last->next && last != new)
		last = last->next;
	same = last == new && !new->next && (head ? lst == head : lst == new);
	try_free_list(lst);
	return (try_expect(same, "new as the last node"));
}
