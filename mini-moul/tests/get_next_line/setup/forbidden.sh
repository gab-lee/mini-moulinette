#!/bin/bash

# Checks the mandatory files for global variables and functions the
# subject does not allow.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_check_symbols mandatory
exit $?
