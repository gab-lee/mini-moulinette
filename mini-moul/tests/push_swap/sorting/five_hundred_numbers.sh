#!/bin/bash

# 500 random numbers. The evaluation scale: under 5500 instructions is
# 5/5, under 7000 4/5, under 8500 3/5, under 10000 2/5, under 11500 1/5. A
# list at or above 11500 fails here; anything below full marks is a [!].
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

ps_random_sorts 500 11500 1 3
exit $pc_error
