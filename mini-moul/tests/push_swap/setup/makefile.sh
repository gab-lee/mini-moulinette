#!/bin/bash

# Checks the Makefile against the subject: it builds push_swap with
# -Wall -Wextra -Werror without relinking and has NAME, all, clean, fclean
# and re rules.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

pc_check_makefile .. push_swap push_swap all clean fclean re
exit $pc_error
