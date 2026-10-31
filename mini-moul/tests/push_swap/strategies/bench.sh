#!/bin/bash

# The --bench mode (subject VI.5): after sorting, stderr gets the disorder
# (percentage, two decimals), the strategy name with its complexity class,
# the total number of operations and the count of each of the 11
# operations. Nothing goes to stderr without the flag, and the operations
# on stdout are the same with and without it.
# The subject does not fix the layout, so the numbers are looked for next
# to their label ("sa: 3", "sa = 3", "sa 3"). The flag goes after the
# selector; the opposite order is tried when that is refused.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

# label|selector|regex of the complexity class
cases=(
	"--simple|--simple|n *(\^ *2|\*\* *2|²|\^\{2\})"
	"--medium|--medium|(√|sqrt)"
	"--complex|--complex|log"
	"--adaptive|--adaptive|(n *(\^ *2|²)|√|sqrt|log)"
	"default selection||(n *(\^ *2|²)|√|sqrt|log)"
)
# shellcheck disable=SC2207
args=($("$PS_TOOL" gen 100 5))
expected_disorder="$("$PS_TOOL" disorder "${args[@]}")"
for entry in "${cases[@]}"; do
	IFS='|' read -r label selector complexity <<< "$entry"

	ps_run $selector "${args[@]}"
	cp .ps_out .ps_plain
	if [ -s .ps_err ]; then
		pc_bad "$label: stderr must stay empty without --bench"
		continue
	fi

	ps_run $selector --bench "${args[@]}"
	if [ "$ps_code" -ne 0 ] || [ ! -s .ps_err ]; then
		ps_run --bench $selector "${args[@]}"
	fi
	problem="$(ps_crash_text)"
	if [ -n "$problem" ]; then
		pc_bad "$label --bench: $problem"
		continue
	fi
	if [ ! -s .ps_err ]; then
		pc_bad "$label --bench: nothing on stderr (exit status $ps_code)"
		continue
	fi
	if ! cmp -s .ps_out .ps_plain; then
		pc_bad "$label --bench: stdout differs from the run without --bench (benchmark output must go to stderr)"
		continue
	fi
	pc_ok "$label --bench: operations stay on stdout, benchmark on stderr"

	if grep -Eq "(^|[^0-9.])${expected_disorder//./\\.}[[:space:]]*%" .ps_err; then
		pc_ok "$label --bench: disorder ${expected_disorder}%"
	else
		pc_bad "$label --bench: disorder should be ${expected_disorder}% (two decimals, with %)"
		pc_detail "stderr starts: $(head -c 80 .ps_err | head -1)"
	fi

	if grep -Eiq "$complexity" .ps_err; then
		pc_ok "$label --bench: complexity class shown"
	else
		pc_bad "$label --bench: the complexity class of the strategy used is not shown"
	fi

	total=$(wc -l < .ps_out | tr -d ' ')
	if grep -Ei 'total' .ps_err | grep -Eq "(^|[^0-9])${total}([^0-9]|$)"; then
		pc_ok "$label --bench: total of $total operations"
	else
		pc_bad "$label --bench: the line with the total should say $total operations"
	fi

	missing=""
	for op in sa sb ss pa pb ra rb rr rra rrb rrr; do
		want=$(grep -cx "$op" .ps_out)
		if ! grep -Eiq "(^|[^a-z])${op}[^a-z0-9]{0,6}${want}([^0-9]|$)" .ps_err; then
			missing="$missing $op=$want"
		fi
	done
	if [ -z "$missing" ]; then
		pc_ok "$label --bench: count of each of the 11 operations"
	else
		pc_bad "$label --bench: missing or wrong operation counts:$missing"
	fi
done
exit $pc_error
