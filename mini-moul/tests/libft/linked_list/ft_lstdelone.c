#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static int	g_del_calls;
static void	*g_last_content;

static void	track_del(void *content)
{
	g_del_calls++;
	g_last_content = content;
	free(content);
}

int main(void)
{
	int		error = 0;
	t_list	*node;
	t_list	*next;
	int		*content;

	content = malloc(sizeof(int));
	*content = 42;
	node = ft_lstnew(content);
	next = ft_lstnew(NULL);
	node->next = next;

	g_del_calls = 0;
	g_last_content = NULL;
	ft_lstdelone(node, track_del);
	if (g_del_calls == 1 && g_last_content == content)
		printf("  " GREEN CHECKMARK GREY " [1] ft_lstdelone calls del exactly once on the node's content\n" DEFAULT);
	else
	{
		printf("    " RED "[1] ft_lstdelone did not call del correctly (calls=%d)\n" DEFAULT, g_del_calls);
		error -= 1;
	}
	printf("  " GREEN CHECKMARK GREY " [2] ft_lstdelone did not crash freeing the node\n" DEFAULT);

	free(next);
	return (error);
}
