#!/bin/bash

# Bonus test cases ported from Tripouille/gnlTester (multiple fds).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_run bonus tripouille_multi
exit $?
