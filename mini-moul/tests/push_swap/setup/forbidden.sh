#!/bin/bash

# The subject allows read, write, malloc, free, exit (and ft_printf or an
# equivalent you coded yourself) and forbids global variables.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

pc_ensure_built .. push_swap || exit 1
pc_check_forbidden .. push_swap 0 "read write malloc free exit" "memset memcpy memmove bzero puts putchar"
exit $pc_error
