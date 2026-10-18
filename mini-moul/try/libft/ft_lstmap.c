#include "../try.h"

/* A new string: the chosen callback applied to each character */
static void	*map(void *content)
{
	char			*s;
	unsigned int	i;

	if (!content)
		return (NULL);
	s = strdup(content);
	i = 0;
	while (s && s[i])
	{
		s[i] = try_map_apply(i, s[i]);
		i++;
	}
	return (s);
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
	t_list	*original;
	t_list	*expected;
	t_list	*node;
	t_list	*mine;
	void	*tmp;
	int		same;

	try_args(argc, 1, 255, "lstmap <" TRY_MAP_NAMES "> [node ...]");
	*try_map_choice() = try_map_id(argv[1]);
	lst = try_list(argc - 2, argv + 2);
	original = try_list(argc - 2, argv + 2);
	expected = try_list(argc - 2, argv + 2);
	printf(TRY_BOLD "ft_lstmap" DEFAULT "(lst, %s, del) with lst ", argv[1]);
	try_put_list(lst);
	mine = ft_lstmap(lst, map, try_del);
	try_row("ft");
	try_put_list(mine);
	node = expected;
	while (node)
	{
		tmp = map(node->content);
		free(node->content);
		node->content = tmp;
		node = node->next;
	}
	try_row("expect");
	try_put_list(expected);
	if (!same_lists(lst, original))
		printf("  " RED "the original list was changed\n" DEFAULT);
	same = same_lists(mine, expected) && same_lists(lst, original)
		&& (mine != lst || !lst);
	try_free_list(mine);
	try_free_list(lst);
	try_free_list(original);
	try_free_list(expected);
	return (try_verdict(same, "the expected new list"));
}
