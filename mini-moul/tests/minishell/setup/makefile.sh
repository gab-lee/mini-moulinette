#!/bin/bash

# Checks the Makefile against the subject: it builds minishell with
# -Wall -Wextra -Werror without relinking and has NAME, all, clean, fclean
# and re rules.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

pc_check_makefile .. minishell minishell all clean fclean re
exit $pc_error
