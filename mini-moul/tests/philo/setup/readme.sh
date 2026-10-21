#!/bin/bash

# Checks the README.md the subject requires (chapter VII), at the root of
# the repository: the folder that holds philo/ and philo_bonus/ (or the
# philo folder itself when it is the whole repository).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_locate
readme="$PH_DIR/../README.md"
[ -f "$PH_DIR/README.md" ] && readme="$PH_DIR/README.md"
pc_check_readme "$readme"
exit $pc_error
