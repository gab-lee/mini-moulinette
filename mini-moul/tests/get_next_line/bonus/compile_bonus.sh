#!/bin/bash

# Builds the bonus files with every tested BUFFER_SIZE (and without
# -D BUFFER_SIZE); every later bonus test runs these builds.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_build bonus
exit $?
