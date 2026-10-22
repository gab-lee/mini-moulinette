#!/bin/bash

# Lists in reverse order. Only the lowest point of the evaluation scale
# applies here (1500 instructions for 100 numbers, 11500 for 500).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

for entry in "5 12" "100 1500" "500 11500"; do
	count="${entry% *}"
	limit="${entry#* }"
	list="$("$PS_TOOL" gen "$count" 5 extremes | tr ' ' '\n' | sort -rn | paste -sd ' ' -)"
	if [ -z "$list" ]; then
		pc_bad "the test helper produced no list of $count numbers"
		continue
	fi
	# shellcheck disable=SC2206
	args=($list)
	ps_sort_case "${args[@]}"
	if [ -z "$ps_problem" ] && [ "$ps_ops" -ge "$limit" ] && [ "$count" -gt 5 ]; then
		ps_problem="$ps_ops operations (must be under $limit)"
	elif [ -z "$ps_problem" ] && [ "$ps_ops" -gt "$limit" ]; then
		ps_problem="$ps_ops operations (at most $limit allowed)"
	fi
	if [ -n "$ps_problem" ]; then
		pc_bad "$count numbers in reverse order: $ps_problem"
	else
		pc_ok "$count numbers in reverse order: sorted in $ps_ops operations"
	fi
done
exit $pc_error
