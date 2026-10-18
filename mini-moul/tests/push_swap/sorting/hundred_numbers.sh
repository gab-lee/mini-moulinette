#!/bin/bash

# 100 random numbers. The evaluation scale: under 700 instructions is
# 5/5, under 900 4/5, under 1100 3/5, under 1300 2/5, under 1500 1/5. A
# list at or above 1500 fails here; anything below full marks is a [!].
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

ps_random_sorts 100 1500 1 5
exit $pc_error
