#!/bin/bash

# Two numbers: sorted with at most one instruction.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

for args in "2 1" "-1 -2" "2147483647 -2147483648"; do
	# shellcheck disable=SC2206
	list=($args)
	ps_sort_case "${list[@]}"
	if [ -z "$ps_problem" ] && [ "$ps_ops" -gt 1 ]; then
		ps_problem="$ps_ops operations (one is enough)"
	fi
	if [ -n "$ps_problem" ]; then
		pc_bad "./push_swap $args: $ps_problem"
	else
		pc_ok "./push_swap $args: $ps_ops operation"
	fi
done
exit $pc_error
