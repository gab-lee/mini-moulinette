#!/bin/bash

# Checks that ft_printf is declared in the student's ft_printf.h with the
# exact prototype the subject defines. Run by test.sh with mini-moul as
# cwd; the student's project is at ../

source ./config.sh
source ./utils/proto_check.sh

check_prototypes "ft_printf.h" \
	"ft_printf|int (*p)(const char *, ...)|int ft_printf(const char *format, ...)"
exit $?
