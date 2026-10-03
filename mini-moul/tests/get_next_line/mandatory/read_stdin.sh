#!/bin/bash

# Reads standard input and pipes with every BUFFER_SIZE.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_run mandatory stdin
exit $?
