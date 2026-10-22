#!/bin/bash

# The bonus version must not crash or leak when a malloc fails either; see
# mandatory/malloc_failure.sh.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_run bonus malloc
exit $?
