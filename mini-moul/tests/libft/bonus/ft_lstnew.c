#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"

int main(void)
{
	int		error = 0;
	int		value = 42;
	t_list	*node;

	node = ft_lstnew(&value);
	if (node == NULL)
	{
		printf("    " RED "[1] ft_lstnew(&value) returned NULL\n" DEFAULT);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [1] ft_lstnew allocates a node\n" DEFAULT);
	if (node->content == &value)
		printf("  " GREEN CHECKMARK GREY " [2] ft_lstnew stores the given content\n" DEFAULT);
	else
	{
		printf("    " RED "[2] ft_lstnew did not store the content pointer\n" DEFAULT);
		error -= 1;
	}
	if (node->next == NULL)
		printf("  " GREEN CHECKMARK GREY " [3] ft_lstnew initializes next to NULL\n" DEFAULT);
	else
	{
		printf("    " RED "[3] ft_lstnew must initialize next to NULL\n" DEFAULT);
		error -= 1;
	}
	free(node);

	node = ft_lstnew(NULL);
	if (node != NULL && node->content == NULL)
		printf("  " GREEN CHECKMARK GREY " [4] ft_lstnew(NULL) still allocates a node with NULL content\n" DEFAULT);
	else
	{
		printf("    " RED "[4] ft_lstnew(NULL) failed to allocate or store NULL content\n" DEFAULT);
		error -= 1;
	}
	free(node);

	return (error);
}
