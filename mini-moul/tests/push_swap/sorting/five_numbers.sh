#!/bin/bash

# Five numbers: every order is sorted with at most 12 instructions.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

ps_every_permutation 5 12
exit $pc_error
