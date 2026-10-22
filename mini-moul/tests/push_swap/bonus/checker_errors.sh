#!/bin/bash

# checker prints "Error" on stderr (and nothing on stdout) for an invalid
# list or an instruction that does not exist or is badly formatted, and
# nothing at all when it gets no argument.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare checker

# label|arguments|instructions (printf format)
cases=(
	"a letter|a|"
	"a duplicate|1 2 1|"
	"INT_MAX + 1|2147483648|"
	"INT_MIN - 1|-2147483649 1|"
	"a huge number|99999999999999999999 1|"
	"an unknown instruction|3 2 1|sx\n"
	"an upper case instruction|3 2 1|SA\n"
	"a trailing space|3 2 1|sa \n"
	"a leading space|3 2 1|\040sa\n"
	"two instructions on one line|3 2 1|sa ra\n"
	"an empty line|3 2 1|sa\n\nra\n"
	"a bad instruction after valid ones|3 2 1|sa\nra\nrra\nxx\n"
	"rrx|3 2 1|rrx\n"
)
for entry in "${cases[@]}"; do
	label="${entry%%|*}"
	rest="${entry#*|}"
	# shellcheck disable=SC2206
	args=(${rest%%|*})
	printf "${rest#*|}" > .ps_in
	ps_run_input .ps_in "${args[@]}"
	problem="$(ps_crash_text)"
	if [ -n "$problem" ]; then
		pc_bad "$label: $problem"
	elif [ -s .ps_out ]; then
		pc_bad "$label: stdout must stay empty"
		pc_detail "stdout: $(head -c 60 .ps_out | head -1)"
	elif [ "$(cat .ps_err)" != "Error" ]; then
		pc_bad "$label: stderr must be exactly \"Error\" and a newline"
		pc_detail "stderr: $(head -c 60 .ps_err | head -1)"
	else
		pc_ok "$label: Error"
	fi
done
rm -f .ps_in

ps_run
problem="$(ps_crash_text)"
if [ -n "$problem" ]; then
	pc_bad "no argument: $problem"
elif [ -s .ps_out ] || [ -s .ps_err ]; then
	pc_bad "no argument: nothing must be displayed"
else
	pc_ok "no argument: nothing displayed"
fi
exit $pc_error
