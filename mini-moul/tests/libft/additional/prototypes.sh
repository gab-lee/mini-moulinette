#!/bin/bash

# Checks that every additional-part function is declared in the
# student's libft.h with the prototype the subject defines.
# Run by test.sh with mini-moul as cwd.

source ./config.sh
source ./utils/proto_check.sh

check_prototypes "libft.h" \
	"ft_substr|char *(*p)(char const *, unsigned int, size_t)|char *ft_substr(char const *s, unsigned int start, size_t len)" \
	"ft_strjoin|char *(*p)(char const *, char const *)|char *ft_strjoin(char const *s1, char const *s2)" \
	"ft_strtrim|char *(*p)(char const *, char const *)|char *ft_strtrim(char const *s1, char const *set)" \
	"ft_split|char **(*p)(char const *, char)|char **ft_split(char const *s, char c)" \
	"ft_itoa|char *(*p)(int)|char *ft_itoa(int n)" \
	"ft_strmapi|char *(*p)(char const *, char (*)(unsigned int, char))|char *ft_strmapi(char const *s, char (*f)(unsigned int, char))" \
	"ft_striteri|void (*p)(char *, void (*)(unsigned int, char *))|void ft_striteri(char *s, void (*f)(unsigned int, char *))" \
	"ft_putchar_fd|void (*p)(char, int)|void ft_putchar_fd(char c, int fd)" \
	"ft_putstr_fd|void (*p)(char *, int)|void ft_putstr_fd(char *s, int fd)" \
	"ft_putendl_fd|void (*p)(char *, int)|void ft_putendl_fd(char *s, int fd)" \
	"ft_putnbr_fd|void (*p)(int, int)|void ft_putnbr_fd(int n, int fd)"
exit $?
