#!/bin/bash

# When a philosopher cannot eat in time, the death is displayed within
# 10 ms of time_to_die after the start of their last meal, nothing is printed
# afterwards, and the program stops.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo

ph_scenario "4 310 200 100" "$PH_BIN" die 5 1 4 310 200 100
ph_scenario "2 300 200 200" "$PH_BIN" die 5 1 2 300 200 200
ph_scenario "3 500 200 200" "$PH_BIN" die 5 1 3 500 200 200
exit $pc_error
