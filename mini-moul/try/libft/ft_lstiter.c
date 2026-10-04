#include "../try.h"

/* Applies the chosen callback to each character of a node's string */
static void	iter(void *content)
{
	char			*s;
	unsigned int	i;

	s = content;
	i = 0;
	while (s && s[i])
	{
		s[i] = try_map_apply(i, s[i]);
		i++;
	}
}

static int	same_lists(t_list *a, t_list *b)
{
	while (a && b)
	{
		if (!a->content != !b->content
			|| (a->content && strcmp(a->content, b->content)))
			return (0);
		a = a->next;
		b = b->next;
	}
	return (!a && !b);
}

int	main(int argc, char **argv)
{
	t_list	*lst;
	t_list	*expected;
	t_list	*node;
	int		same;

	try_args(argc, 1, 255, "lstiter <" TRY_MAP_NAMES "> [node ...]");
	*try_map_choice() = try_map_id(argv[1]);
	lst = try_list(argc - 2, argv + 2);
	expected = try_list(argc - 2, argv + 2);
	printf(TRY_BOLD "ft_lstiter" DEFAULT "(lst, %s) with lst ", argv[1]);
	try_put_list(lst);
	ft_lstiter(lst, iter);
	try_row("ft");
	try_put_list(lst);
	node = expected;
	while (node)
	{
		iter(node->content);
		node = node->next;
	}
	try_row("expect");
	try_put_list(expected);
	same = same_lists(lst, expected);
	try_free_list(lst);
	try_free_list(expected);
	return (try_verdict(same, "the expected list"));
}
