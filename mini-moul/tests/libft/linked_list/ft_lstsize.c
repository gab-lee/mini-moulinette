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

static int	lstsize_case(int i, char *desc, t_list *lst, int expected)
{
	int	res;

	res = ft_lstsize(lst);
	if (res == expected)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
		return (0);
	}
	printf("    " RED "[%d] %s: expected %d, got %d\n" DEFAULT,
		i, desc, expected, res);
	return (-1);
}

int main(void)
{
	int		error = 0;
	int		value = 1;
	t_list	*empty;
	t_list	*one;
	t_list	*three;

	empty = NULL;
	error += lstsize_case(1, "ft_lstsize(NULL) returns 0", empty, 0);

	one = ft_lstnew(&value);
	error += lstsize_case(2, "ft_lstsize on a single node returns 1", one, 1);

	three = ft_lstnew(&value);
	three->next = ft_lstnew(&value);
	three->next->next = ft_lstnew(&value);
	error += lstsize_case(3, "ft_lstsize on a 3-node list returns 3", three, 3);

	free_list(one);
	free_list(three);
	return (error);
}
