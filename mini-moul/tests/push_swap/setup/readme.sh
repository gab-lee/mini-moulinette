#!/bin/bash

# Checks the README.md the subject requires (chapter VII) at the root of
# the repository: italic first line, Description, Instructions and
# Resources sections. The algorithm explanation and the thresholds
# rationale of the adaptive strategy cannot be checked.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

pc_check_readme ../README.md
exit $pc_error
