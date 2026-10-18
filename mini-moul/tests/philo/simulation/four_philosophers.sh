#!/bin/bash

# 4 410 200 200 leaves 10 ms of slack: a philosopher must not die. The
# program runs for $PH_ALIVE seconds (8 by default) and has to still be
# running.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo

ph_scenario "4 410 200 200" "$PH_BIN" alive "$PH_ALIVE" 1 4 410 200 200
exit $pc_error
