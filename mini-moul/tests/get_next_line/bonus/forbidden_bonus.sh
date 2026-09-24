#!/bin/bash

# Checks the bonus files for global variables, functions the subject
# does not allow, and more than the one static variable the bonus allows.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_check_symbols bonus 1
exit $?
