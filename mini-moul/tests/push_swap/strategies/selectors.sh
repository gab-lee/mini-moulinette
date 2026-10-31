#!/bin/bash

# The strategy selectors of the subject (VI.5): --simple, --medium,
# --complex and --adaptive (the default) must sort every valid input,
# whatever its size or disorder, and report errors like the default run.
# No operation limit applies: a forced O(n^2) run may need more than the
# benchmark allows.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

for flag in --simple --medium --complex --adaptive; do
	for count in 2 3 5 100 500; do
		for seed in 1 2; do
			[ "$count" -ge 100 ] || [ "$seed" -eq 1 ] || continue
			[ "$count" -eq 500 ] && [ "$seed" -eq 2 ] && continue
			# shellcheck disable=SC2207
			args=($("$PS_TOOL" gen "$count" "$seed"))
			ps_sort_case "$flag" "${args[@]}"
			if [ -n "$ps_problem" ]; then
				pc_bad "$flag with $count random numbers: $ps_problem"
			else
				pc_ok "$flag with $count random numbers: sorted in $ps_ops operations"
			fi
		done
	done

	# Disorder 0 and 1: the extremes of the metric
	# shellcheck disable=SC2207
	args=($("$PS_TOOL" gen 100 3 | tr ' ' '\n' | sort -n | paste -sd ' ' -))
	ps_run "$flag" "${args[@]}"
	problem="$(ps_crash_text)"
	if [ -n "$problem" ]; then
		pc_bad "$flag with 100 sorted numbers: $problem"
	elif [ -s .ps_out ] || [ -s .ps_err ]; then
		pc_bad "$flag with 100 sorted numbers: nothing should be displayed"
	else
		pc_ok "$flag with 100 sorted numbers: nothing displayed"
	fi
	# shellcheck disable=SC2207
	args=($("$PS_TOOL" gen 100 3 | tr ' ' '\n' | sort -rn | paste -sd ' ' -))
	ps_sort_case "$flag" "${args[@]}"
	if [ -n "$ps_problem" ]; then
		pc_bad "$flag with 100 numbers in reverse order: $ps_problem"
	else
		pc_ok "$flag with 100 numbers in reverse order: sorted in $ps_ops operations"
	fi

	# Errors with a selector (subject examples)
	for entry in "0 one 2 3" "3 2 3" "1 2147483648"; do
		# shellcheck disable=SC2206
		args=($entry)
		ps_run "$flag" "${args[@]}"
		problem="$(ps_crash_text)"
		if [ -n "$problem" ]; then
			pc_bad "$flag $entry: $problem"
		elif [ -s .ps_out ] || [ "$(cat .ps_err)" != "Error" ]; then
			pc_bad "$flag $entry: must display only \"Error\" on stderr"
		else
			pc_ok "$flag $entry: Error"
		fi
	done
done
exit $pc_error
