#!/bin/bash

# Same rules as push_swap: read, write, malloc, free, exit and your own
# functions only, and no global variable.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

pc_ensure_built .. checker bonus || exit 1
pc_check_forbidden .. checker 0 "read write malloc free exit" "memset memcpy memmove bzero puts putchar"
exit $pc_error
