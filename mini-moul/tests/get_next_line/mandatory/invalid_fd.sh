#!/bin/bash

# Invalid fds and read() errors must return NULL without leaking.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_run mandatory errors
exit $?
