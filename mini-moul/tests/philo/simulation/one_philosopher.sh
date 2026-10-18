#!/bin/bash

# One philosopher has a single fork and must die after time_to_die. The
# message has to appear within 10 ms, and the program has to stop.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo

ph_scenario "1 800 200 200" "$PH_BIN" die 5 1 1 800 200 200
exit $pc_error
