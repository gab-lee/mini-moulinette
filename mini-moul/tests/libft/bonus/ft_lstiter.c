#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static void	double_it(void *content)
{
	*(int *)content *= 2;
}

static int	g_iter_calls;

static void	count_it(void *content)
{
	(void)content;
	g_iter_calls++;
}

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

	lst = ft_lstnew(&values[0]);
	lst->next = ft_lstnew(&values[1]);
	lst->next->next = ft_lstnew(&values[2]);

	ft_lstiter(lst, double_it);
	if (values[0] == 2 && values[1] == 4 && values[2] == 6)
		printf("  " GREEN CHECKMARK GREY " [1] ft_lstiter applies f to every node's content\n" DEFAULT);
	else
	{
		printf("    " RED "[1] ft_lstiter did not apply f to every content (%d, %d, %d)\n" DEFAULT,
			values[0], values[1], values[2]);
		error -= 1;
	}

	g_iter_calls = 0;
	ft_lstiter(NULL, count_it);
	if (g_iter_calls == 0)
		printf("  " GREEN CHECKMARK GREY " [2] ft_lstiter(NULL, f) does not call f and does not crash\n" DEFAULT);
	else
	{
		printf("    " RED "[2] ft_lstiter(NULL, f) should not call f\n" DEFAULT);
		error -= 1;
	}

	free_list(lst);
	return (error);
}
