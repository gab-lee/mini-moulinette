#!/bin/bash

# With number_of_times_each_philosopher_must_eat the simulation stops once
# every philosopher has eaten that many times, without anyone dying.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo

ph_scenario "5 800 200 200 7" "$PH_BIN" count:7 "$PH_LIMIT" 1 5 800 200 200 7
ph_scenario "4 410 200 200 5" "$PH_BIN" count:5 "$PH_LIMIT" 1 4 410 200 200 5
ph_scenario "20 800 200 200 3" "$PH_BIN" count:3 "$PH_LIMIT" 1 20 800 200 200 3
exit $pc_error
