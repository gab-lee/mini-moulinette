#!/bin/bash

# Checks that every libc-part function is declared in the student's
# libft.h with the prototype the subject defines — not the one from
# the libc man pages. Run by test.sh with mini-moul as cwd.

source ./config.sh
source ./utils/proto_check.sh

check_prototypes \
	"ft_isalpha|int (*p)(int)|int ft_isalpha(int c)" \
	"ft_isdigit|int (*p)(int)|int ft_isdigit(int c)" \
	"ft_isalnum|int (*p)(int)|int ft_isalnum(int c)" \
	"ft_isascii|int (*p)(int)|int ft_isascii(int c)" \
	"ft_isprint|int (*p)(int)|int ft_isprint(int c)" \
	"ft_strlen|size_t (*p)(const char *)|size_t ft_strlen(const char *s)" \
	"ft_memset|void *(*p)(void *, int, size_t)|void *ft_memset(void *b, int c, size_t len)" \
	"ft_bzero|void (*p)(void *, size_t)|void ft_bzero(void *s, size_t n)" \
	"ft_memcpy|void *(*p)(void *, const void *, size_t)|void *ft_memcpy(void *dst, const void *src, size_t n)" \
	"ft_memmove|void *(*p)(void *, const void *, size_t)|void *ft_memmove(void *dst, const void *src, size_t len)" \
	"ft_strlcpy|size_t (*p)(char *, const char *, size_t)|size_t ft_strlcpy(char *dst, const char *src, size_t dstsize)" \
	"ft_strlcat|size_t (*p)(char *, const char *, size_t)|size_t ft_strlcat(char *dst, const char *src, size_t dstsize)" \
	"ft_toupper|int (*p)(int)|int ft_toupper(int c)" \
	"ft_tolower|int (*p)(int)|int ft_tolower(int c)" \
	"ft_strchr|char *(*p)(const char *, int)|char *ft_strchr(const char *s, int c)" \
	"ft_strrchr|char *(*p)(const char *, int)|char *ft_strrchr(const char *s, int c)" \
	"ft_strncmp|int (*p)(const char *, const char *, size_t)|int ft_strncmp(const char *s1, const char *s2, size_t n)" \
	"ft_memchr|void *(*p)(const void *, int, size_t)|void *ft_memchr(const void *s, int c, size_t n)" \
	"ft_memcmp|int (*p)(const void *, const void *, size_t)|int ft_memcmp(const void *s1, const void *s2, size_t n)" \
	"ft_strnstr|char *(*p)(const char *, const char *, size_t)|char *ft_strnstr(const char *haystack, const char *needle, size_t len)" \
	"ft_atoi|int (*p)(const char *)|int ft_atoi(const char *str)" \
	"ft_calloc|void *(*p)(size_t, size_t)|void *ft_calloc(size_t count, size_t size)" \
	"ft_strdup|char *(*p)(const char *)|char *ft_strdup(const char *s1)"
exit $?
