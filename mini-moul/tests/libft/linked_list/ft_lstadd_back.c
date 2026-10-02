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
	int		values[5] = {1, 2, 3, 4, 5};
	t_list	*lst;
	t_list	*a;
	t_list	*b;
	t_list	*other;
	t_list	*cur;
	int		k;

	lst = NULL;
	a = ft_lstnew(&values[0]);
	ft_lstadd_back(&lst, a);
	if (lst == a && lst->next == NULL)
		printf("  " GREEN CHECKMARK GREY " [1] ft_lstadd_back on an empty list becomes the only node\n" DEFAULT);
	else
	{
		printf("    " RED "[1] ft_lstadd_back on an empty list is broken\n" DEFAULT);
		error -= 1;
	}

	b = ft_lstnew(&values[1]);
	ft_lstadd_back(&lst, b);
	if (lst == a && a->next == b && b->next == NULL)
		printf("  " GREEN CHECKMARK GREY " [2] ft_lstadd_back appends after the existing node\n" DEFAULT);
	else
	{
		printf("    " RED "[2] ft_lstadd_back did not append at the tail correctly\n" DEFAULT);
		error -= 1;
	}

	ft_lstadd_back(&lst, ft_lstnew(&values[2]));
	if (ft_lstsize(lst) == 3 && ft_lstlast(lst)->content == &values[2])
		printf("  " GREEN CHECKMARK GREY " [3] ft_lstadd_back keeps growing the list correctly\n" DEFAULT);
	else
	{
		printf("    " RED "[3] ft_lstadd_back broke the list on a third insert\n" DEFAULT);
		error -= 1;
	}

	other = NULL;
	ft_lstadd_back(&other, ft_lstnew(&values[3]));
	ft_lstadd_back(&other, ft_lstnew(&values[4]));
	ft_lstadd_back(&lst, other);
	cur = lst;
	k = 0;
	while (cur && k < 5 && cur->content == &values[k])
	{
		cur = cur->next;
		k++;
	}
	if (k == 5 && cur == NULL)
		printf("  " GREEN CHECKMARK GREY " [4] ft_lstadd_back appends a whole second list, keeping its nodes\n" DEFAULT);
	else
	{
		printf("    " RED "[4] ft_lstadd_back of a two-node list: expected contents 1,2,3,4,5 then NULL\n" DEFAULT);
		error -= 1;
	}

	free_list(lst);
	return (error);
}
