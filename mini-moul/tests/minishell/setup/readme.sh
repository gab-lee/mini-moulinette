#!/bin/bash

# Checks the README.md the subject requires (chapter V) at the root of the
# repository.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

pc_check_readme ../README.md
exit $pc_error
