#!/bin/bash

# Checks that every bonus-part function is declared in the student's
# libft.h with the prototype the subject defines (using their own
# t_list, which libft.h must provide for the bonus part).
# Run by test.sh with mini-moul as cwd.

source ./config.sh
source ./utils/proto_check.sh

check_prototypes "libft.h" \
	"ft_lstnew|t_list *(*p)(void *)|t_list *ft_lstnew(void *content)" \
	"ft_lstadd_front|void (*p)(t_list **, t_list *)|void ft_lstadd_front(t_list **lst, t_list *new)" \
	"ft_lstsize|int (*p)(t_list *)|int ft_lstsize(t_list *lst)" \
	"ft_lstlast|t_list *(*p)(t_list *)|t_list *ft_lstlast(t_list *lst)" \
	"ft_lstadd_back|void (*p)(t_list **, t_list *)|void ft_lstadd_back(t_list **lst, t_list *new)" \
	"ft_lstdelone|void (*p)(t_list *, void (*)(void *))|void ft_lstdelone(t_list *lst, void (*del)(void *))" \
	"ft_lstclear|void (*p)(t_list **, void (*)(void *))|void ft_lstclear(t_list **lst, void (*del)(void *))" \
	"ft_lstiter|void (*p)(t_list *, void (*)(void *))|void ft_lstiter(t_list *lst, void (*f)(void *))" \
	"ft_lstmap|t_list *(*p)(t_list *, void *(*)(void *), void (*)(void *))|t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))"
exit $?
