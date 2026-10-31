#!/bin/bash

# 500 random numbers. The subject's benchmark (VI.6): under 12000
# instructions passes, under 8000 is good, under 5500 is excellent. A list
# at or above 12000 fails here; anything below excellent is a [!].
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

ps_random_sorts 500 1 3
exit $pc_error
