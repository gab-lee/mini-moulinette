#!/bin/bash

# Checks the README.md the subject requires (chapter V).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_check_readme
exit $?
