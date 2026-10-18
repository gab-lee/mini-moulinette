#!/bin/bash

# Your push_swap piped into your checker must give OK; cut short, KO.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare checker
pc_ensure_built .. push_swap || exit 1

for count in 3 5 100 500; do
	list="$("$PS_TOOL" gen "$count" 9)"
	if [ -z "$list" ]; then
		pc_bad "the test helper produced no list of $count numbers"
		continue
	fi
	# shellcheck disable=SC2206
	args=($list)
	pc_limited "$PS_TIMEOUT" ../push_swap "${args[@]}" > .ps_in 2> /dev/null
	ps_run_input .ps_in "${args[@]}"
	problem="$(ps_crash_text)"
	if [ -n "$problem" ]; then
		pc_bad "$count numbers, push_swap | checker: $problem"
	elif [ "$(cat .ps_out)" != "OK" ]; then
		pc_bad "$count numbers, push_swap | checker: expected OK"
		pc_detail "stdout: $(head -c 60 .ps_out | head -1)"
	else
		pc_ok "$count numbers, push_swap | checker: OK"
	fi
	if [ -s .ps_in ]; then
		sed '1d' .ps_in > .ps_in2
		ps_run_input .ps_in2 "${args[@]}"
		if [ "$(cat .ps_out)" = "KO" ]; then
			pc_ok "$count numbers, first instruction dropped: KO"
		else
			pc_bad "$count numbers, first instruction dropped: expected KO"
			pc_detail "stdout: $(head -c 60 .ps_out | head -1)"
		fi
	fi
done
rm -f .ps_in .ps_in2
exit $pc_error
