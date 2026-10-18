#!/bin/bash

# Checks the Makefile against the subject: it builds philo with
# -Wall -Wextra -Werror without relinking and has NAME, all, clean, fclean
# and re rules.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_locate
pc_check_makefile "$PH_DIR" philo philo all clean fclean re
exit $pc_error
