#!/bin/bash

# Reads regular files line by line with every BUFFER_SIZE.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_run mandatory file
exit $?
