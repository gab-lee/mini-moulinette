#!/bin/bash

# Five philosophers sharing a pool of forks: nobody may die. The program
# runs for $PH_ALIVE seconds (8 by default) and has to still be running.
# Run by test.sh with the mini-moul directory as cwd; the student's
# philo_bonus folder is next to the philo folder.

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo_bonus

ph_scenario "5 800 200 200" "$PH_BIN" alive "$PH_ALIVE" 0 5 800 200 200
exit $pc_error
