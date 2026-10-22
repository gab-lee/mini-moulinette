#!/bin/bash

# One philosopher has a single fork and must die after time_to_die.
# Run by test.sh with the mini-moul directory as cwd; the student's
# philo_bonus folder is next to the philo folder.

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo_bonus

ph_scenario "1 800 200 200" "$PH_BIN" die 5 0 1 800 200 200
exit $pc_error
