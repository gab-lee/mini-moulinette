#!/bin/bash

# A list that is already sorted needs no instruction: nothing is displayed.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

sorted_cases=(
	"one number|42"
	"two sorted numbers|1 2"
	"three sorted numbers|-5 0 7"
	"five sorted numbers|-2147483648 -1 0 1 2147483647"
)
for entry in "${sorted_cases[@]}"; do
	label="${entry%%|*}"
	# shellcheck disable=SC2206
	args=(${entry#*|})
	ps_run "${args[@]}"
	problem="$(ps_crash_text)"
	if [ -n "$problem" ]; then
		pc_bad "$label: $problem"
	elif [ -s .ps_out ]; then
		pc_bad "$label (./push_swap ${args[*]}) is already sorted but instructions were printed"
		pc_detail "stdout: $(head -c 60 .ps_out | head -1)"
	elif [ -s .ps_err ]; then
		pc_bad "$label: nothing should be written on stderr"
		pc_detail "stderr: $(head -c 60 .ps_err | head -1)"
	else
		pc_ok "$label: nothing displayed"
	fi
done

for count in 100 500; do
	list="$("$PS_TOOL" gen "$count" 7 extremes | tr ' ' '\n' | sort -n | paste -sd ' ' -)"
	if [ -z "$list" ]; then
		pc_bad "the test helper produced no list of $count numbers"
		continue
	fi
	# shellcheck disable=SC2206
	args=($list)
	ps_run "${args[@]}"
	problem="$(ps_crash_text)"
	if [ -n "$problem" ]; then
		pc_bad "$count sorted numbers: $problem"
	elif [ -s .ps_out ]; then
		pc_bad "$count numbers already sorted but $(wc -l < .ps_out | tr -d ' ') instruction(s) were printed"
	else
		pc_ok "$count sorted numbers: nothing displayed"
	fi
done
exit $pc_error
