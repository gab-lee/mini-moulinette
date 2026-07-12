#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static void	free_list(t_list *lst)
{
	t_list	*next;

	while (lst)
	{
		next = lst->next;
		free(lst);
		lst = next;
	}
}

int main(void)
{
	int		error = 0;
	int		values[3] = {1, 2, 3};
	t_list	*lst;
	t_list	*a;
	t_list	*b;

	lst = NULL;
	ft_lstadd_front(&lst, ft_lstnew(&values[0]));
	if (lst != NULL && lst->content == &values[0] && lst->next == NULL)
		printf("  " GREEN CHECKMARK GREY " [1] ft_lstadd_front on an empty list becomes the only node\n" DEFAULT);
	else
	{
		printf("    " RED "[1] ft_lstadd_front on an empty list is broken\n" DEFAULT);
		error -= 1;
	}

	a = ft_lstnew(&values[1]);
	ft_lstadd_front(&lst, a);
	if (lst == a && lst->next != NULL && lst->next->content == &values[0])
		printf("  " GREEN CHECKMARK GREY " [2] ft_lstadd_front pushes the new node to the head\n" DEFAULT);
	else
	{
		printf("    " RED "[2] ft_lstadd_front did not push to the head correctly\n" DEFAULT);
		error -= 1;
	}

	b = ft_lstnew(&values[2]);
	ft_lstadd_front(&lst, b);
	if (lst == b && ft_lstsize(lst) == 3)
		printf("  " GREEN CHECKMARK GREY " [3] ft_lstadd_front keeps the rest of the list intact\n" DEFAULT);
	else
	{
		printf("    " RED "[3] ft_lstadd_front broke the rest of the list\n" DEFAULT);
		error -= 1;
	}

	free_list(lst);
	return (error);
}
