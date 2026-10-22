#!/bin/bash

# Lists holding INT_MIN and INT_MAX: an implementation that subtracts
# values to compare them overflows here.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

ps_sort_case 2147483647 -2147483648 0
if [ -z "$ps_problem" ] && [ "$ps_ops" -gt 3 ]; then
	ps_problem="$ps_ops operations (at most 3 allowed)"
fi
if [ -n "$ps_problem" ]; then
	pc_bad "INT_MAX, INT_MIN and 0: $ps_problem"
else
	pc_ok "INT_MAX, INT_MIN and 0: sorted in $ps_ops operation(s)"
fi
ps_every_permutation 5 12
ps_random_sorts 10 100000 21 3 extremes
ps_random_sorts 100 1500 31 2 extremes
exit $pc_error
