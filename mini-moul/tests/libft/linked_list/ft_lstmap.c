#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

static void	*double_it(void *content)
{
	int	*res;

	res = malloc(sizeof(int));
	if (!res)
		return (NULL);
	*res = *(int *)content * 2;
	return (res);
}

static void	free_content(void *content)
{
	free(content);
}

static void	free_nodes(t_list *lst)
{
	t_list	*next;

	while (lst)
	{
		next = lst->next;
		free(lst);
		lst = next;
	}
}

static void	free_mapped(t_list *lst)
{
	t_list	*next;

	while (lst)
	{
		next = lst->next;
		free(lst->content);
		free(lst);
		lst = next;
	}
}

int main(void)
{
	int		error = 0;
	int		values[3] = {1, 2, 3};
	t_list	*lst;
	t_list	*mapped;
	t_list	*empty_mapped;

	lst = ft_lstnew(&values[0]);
	lst->next = ft_lstnew(&values[1]);
	lst->next->next = ft_lstnew(&values[2]);

	mapped = ft_lstmap(lst, double_it, free_content);
	if (mapped != NULL && ft_lstsize(mapped) == 3
		&& *(int *)mapped->content == 2
		&& *(int *)mapped->next->content == 4
		&& *(int *)mapped->next->next->content == 6)
		printf("  " GREEN CHECKMARK GREY " [1] ft_lstmap builds a new list with f applied to each content\n" DEFAULT);
	else
	{
		printf("    " RED "[1] ft_lstmap did not build the expected mapped list\n" DEFAULT);
		error -= 1;
	}
	if (mapped != NULL && mapped != lst && mapped->content != lst->content)
		printf("  " GREEN CHECKMARK GREY " [2] ft_lstmap does not mutate the original list\n" DEFAULT);
	else
	{
		printf("    " RED "[2] ft_lstmap should return a brand new list, not reuse the original\n" DEFAULT);
		error -= 1;
	}
	if (values[0] == 1 && values[1] == 2 && values[2] == 3)
		printf("  " GREEN CHECKMARK GREY " [3] ft_lstmap leaves the original content untouched\n" DEFAULT);
	else
	{
		printf("    " RED "[3] ft_lstmap modified the original list's content\n" DEFAULT);
		error -= 1;
	}

	empty_mapped = ft_lstmap(NULL, double_it, free_content);
	if (empty_mapped == NULL)
		printf("  " GREEN CHECKMARK GREY " [4] ft_lstmap(NULL, f, del) returns NULL\n" DEFAULT);
	else
	{
		printf("    " RED "[4] ft_lstmap(NULL, ...) should return NULL\n" DEFAULT);
		error -= 1;
	}

	free_nodes(lst);
	free_mapped(mapped);
	return (error);
}
