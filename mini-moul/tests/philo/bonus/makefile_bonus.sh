#!/bin/bash

# philo_bonus has its own Makefile: it builds philo_bonus with
# -Wall -Wextra -Werror without relinking and has NAME, all, clean, fclean
# and re rules.
# Run by test.sh with the mini-moul directory as cwd; the student's
# philo_bonus folder is next to the philo folder.

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_locate
if [ ! -f "$PH_BONUS_DIR/Makefile" ]; then
	pc_bad "philo_bonus/Makefile not found next to your philo folder"
	exit 1
fi
pc_check_makefile "$PH_BONUS_DIR" philo_bonus philo_bonus all clean fclean re
exit $pc_error
