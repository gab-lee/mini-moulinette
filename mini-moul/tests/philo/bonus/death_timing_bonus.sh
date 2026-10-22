#!/bin/bash

# The death is displayed within 10 ms of time_to_die, nothing is printed
# afterwards, and the program stops.
# Run by test.sh with the mini-moul directory as cwd; the student's
# philo_bonus folder is next to the philo folder.

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo_bonus

ph_scenario "4 310 200 100" "$PH_BIN" die 5 0 4 310 200 100
ph_scenario "2 300 200 200" "$PH_BIN" die 5 0 2 300 200 200
exit $pc_error
