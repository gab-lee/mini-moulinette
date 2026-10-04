#include "../try.h"

int	main(int argc, char **argv)
{
	char	*content;
	t_list	*node;
	int		same;

	try_args(argc, 1, 1, "lstnew <content>");
	content = try_str(argv[1], NULL);
	try_call("ft_lstnew");
	try_put_str(content);
	try_call_end();
	node = ft_lstnew(content);
	try_row("ft");
	if (!node)
	{
		printf("NULL\n");
		return (try_expect(0, "a new node"));
	}
	printf("node { content ");
	try_put_str(node->content);
	printf("%s, next %s }\n",
		node->content == content ? " (the pointer passed in)" : " (a different pointer)",
		node->next ? "not NULL" : "NULL");
	same = node->content == content && !node->next;
	free(node);
	return (try_expect(same, "a new node holding content, next NULL"));
}
