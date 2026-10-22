#!/bin/bash

# Every invalid list must print "Error" followed by a newline on stderr,
# and no instruction on stdout: a non-integer, a number outside the int
# range, or a duplicate.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

# label|arguments (split on spaces)
cases=(
	"a letter|a"
	"a letter after valid numbers|3 2 1 a"
	"a letter in the middle|3 x 1"
	"trailing letter|12 3x"
	"a decimal number|1.5 2"
	"a lone minus|-"
	"a lone plus|+"
	"two minus signs|--1 2"
	"a minus inside the number|1-2 3"
	"INT_MAX + 1|2147483648"
	"INT_MIN - 1|-2147483649"
	"a huge number|99999999999999999999"
	"a huge negative number|-99999999999999999999"
	"INT_MAX + 1 after valid numbers|3 2 1 2147483648"
	"a number that wraps to 1 in 32 bits|4294967297 2"
	"a duplicate|1 2 1"
	"two equal numbers|5 5"
	"a duplicate at the end|3 1 2 4 3"
	"INT_MAX twice|2147483647 2147483647"
	"0 and -0|0 -0"
	"1 and +1|1 +1"
)

for entry in "${cases[@]}"; do
	label="${entry%%|*}"
	# shellcheck disable=SC2206
	args=(${entry#*|})
	ps_run "${args[@]}"
	problem="$(ps_crash_text)"
	if [ -n "$problem" ]; then
		pc_bad "$label (./push_swap ${args[*]}): $problem"
	elif [ -s .ps_out ]; then
		pc_bad "$label (./push_swap ${args[*]}): printed on stdout instead of failing"
		pc_detail "stdout: $(head -c 60 .ps_out | head -1)"
	elif [ "$(cat .ps_err)" != "Error" ]; then
		pc_bad "$label (./push_swap ${args[*]}): stderr must be exactly \"Error\" and a newline"
		pc_detail "stderr: $(head -c 60 .ps_err | head -1)"
	else
		pc_ok "$label: Error"
	fi
done

# The int limits themselves are valid
ps_sort_case 2147483647 -2147483648
if [ -n "$ps_problem" ]; then
	pc_bad "INT_MAX and INT_MIN are valid numbers (./push_swap 2147483647 -2147483648): $ps_problem"
else
	pc_ok "INT_MAX and INT_MIN are accepted and sorted"
fi
exit $pc_error
