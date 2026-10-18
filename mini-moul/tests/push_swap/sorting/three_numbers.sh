#!/bin/bash

# Three numbers: every order is sorted with at most 3 instructions.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

ps_every_permutation 3 3
exit $pc_error
