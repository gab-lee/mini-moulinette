#!/bin/bash

# The simulation stops once every philosopher has eaten the given number of
# times, without anyone dying.
# Run by test.sh with the mini-moul directory as cwd; the student's
# philo_bonus folder is next to the philo folder.

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo_bonus

ph_scenario "5 800 200 200 7" "$PH_BIN" count:7 "$PH_LIMIT" 0 5 800 200 200 7
ph_scenario "4 410 200 200 5" "$PH_BIN" count:5 "$PH_LIMIT" 0 4 410 200 200 5
exit $pc_error
