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
	t_list	*one;
	t_list	*three;
	t_list	*last;

	if (ft_lstlast(NULL) == NULL)
		printf("  " GREEN CHECKMARK GREY " [1] ft_lstlast(NULL) returns NULL\n" DEFAULT);
	else
	{
		printf("    " RED "[1] ft_lstlast(NULL) must return NULL\n" DEFAULT);
		error -= 1;
	}

	one = ft_lstnew(&values[0]);
	if (ft_lstlast(one) == one)
		printf("  " GREEN CHECKMARK GREY " [2] ft_lstlast on a single node returns that node\n" DEFAULT);
	else
	{
		printf("    " RED "[2] ft_lstlast on a single node did not return itself\n" DEFAULT);
		error -= 1;
	}

	three = ft_lstnew(&values[0]);
	three->next = ft_lstnew(&values[1]);
	three->next->next = ft_lstnew(&values[2]);
	last = ft_lstlast(three);
	if (last == three->next->next && last->next == NULL)
		printf("  " GREEN CHECKMARK GREY " [3] ft_lstlast on a 3-node list returns the tail\n" DEFAULT);
	else
	{
		printf("    " RED "[3] ft_lstlast did not return the tail of the list\n" DEFAULT);
		error -= 1;
	}

	free_list(one);
	free_list(three);
	return (error);
}
