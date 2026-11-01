#!/bin/bash

# 100 random numbers. The subject's benchmark (VI.6): under 2000
# instructions passes, under 1500 is good, under 700 is excellent. A list
# at or above 2000 fails here; anything below excellent is a [!].
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

ps_random_sorts 100 1 5
exit $pc_error
