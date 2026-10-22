#!/bin/bash

# Four numbers: every order must be sorted. The evaluation sets no limit
# here, so the number of instructions is only reported.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

ps_every_permutation 4 100
exit $pc_error
