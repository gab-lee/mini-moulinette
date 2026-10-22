#!/bin/bash

# Two philosophers share two forks and take turns: nobody may die. The
# program runs for $PH_ALIVE seconds (8 by default) and has to still be
# running; the log is checked while it runs.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo

ph_scenario "2 800 200 200" "$PH_BIN" alive "$PH_ALIVE" 1 2 800 200 200
exit $pc_error
