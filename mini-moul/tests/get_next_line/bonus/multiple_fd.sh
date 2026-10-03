#!/bin/bash

# Reads several fds in turn; each must keep its own position.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_run bonus multi
exit $?
