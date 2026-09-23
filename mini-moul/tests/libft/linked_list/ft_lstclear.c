#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static int	g_del_calls;

static void	count_del(void *content)
{
	g_del_calls++;
	free(content);
}

int main(void)
{
	int		error = 0;
	t_list	*lst;
	t_list	*empty;

	lst = ft_lstnew(malloc(sizeof(int)));
	lst->next = ft_lstnew(malloc(sizeof(int)));
	lst->next->next = ft_lstnew(malloc(sizeof(int)));

	g_del_calls = 0;
	ft_lstclear(&lst, count_del);
	if (g_del_calls == 3)
		printf("  " GREEN CHECKMARK GREY " [1] ft_lstclear calls del on every node's content\n" DEFAULT);
	else
	{
		printf("    " RED "[1] ft_lstclear: expected del to be called 3 times, got %d\n" DEFAULT,
			g_del_calls);
		error -= 1;
	}
	if (lst == NULL)
		printf("  " GREEN CHECKMARK GREY " [2] ft_lstclear sets the list pointer to NULL\n" DEFAULT);
	else
	{
		printf("    " RED "[2] ft_lstclear must set *lst to NULL after clearing\n" DEFAULT);
		error -= 1;
	}

	empty = NULL;
	g_del_calls = 0;
	ft_lstclear(&empty, count_del);
	if (empty == NULL && g_del_calls == 0)
		printf("  " GREEN CHECKMARK GREY " [3] ft_lstclear on an already-empty list does nothing and does not crash\n" DEFAULT);
	else
	{
		printf("    " RED "[3] ft_lstclear on an empty list misbehaved\n" DEFAULT);
		error -= 1;
	}

	return (error);
}
