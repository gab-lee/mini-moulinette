#!/bin/bash

# checker prints OK when the instructions leave stack a sorted and stack b
# empty, KO otherwise. Solutions come from a breadth-first search, so
# dropping their last instruction always leaves the stack unsorted.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare checker

# expect <label> <OK|KO> <input file> <args...>
expect()
{
	local label=$1 want=$2 input=$3
	shift 3
	ps_run_input "$input" "$@"
	problem="$(ps_crash_text)"
	if [ -n "$problem" ]; then
		pc_bad "$label: $problem"
	elif [ -s .ps_err ]; then
		pc_bad "$label: nothing must be written on stderr"
		pc_detail "stderr: $(head -c 60 .ps_err | head -1)"
	elif [ "$(cat .ps_out)" != "$want" ]; then
		pc_bad "$label: expected $want"
		pc_detail "stdout: $(head -c 60 .ps_out | head -1)"
	else
		pc_ok "$label: $want"
	fi
}

printf '' > .ps_in
expect "sorted list, no instruction" OK .ps_in 1 2 3
expect "unsorted list, no instruction" KO .ps_in 2 1 3
printf 'sa\nrra\n' > .ps_in
expect "3 2 1 with sa rra" OK .ps_in 3 2 1
printf 'pb\n' > .ps_in
expect "sorted list but b is not empty" KO .ps_in 1 2 3
printf 'pb\npa\n' > .ps_in
expect "sorted list, pb then pa" OK .ps_in 1 2 3
printf 'sa\n' > .ps_in
expect "sa on one number does nothing" OK .ps_in 7
printf 'pa\nsa\n' > .ps_in
expect "pa with an empty b does nothing" OK .ps_in 2 1
printf 'rr\nrrr\nss\nsb\nrb\nrrb\n' > .ps_in
expect "operations on an empty stack b do nothing" OK .ps_in 2 1
printf 'pb\npb\nss\npa\npa\n' > .ps_in
expect "ss swaps both stacks" OK .ps_in 2 1 4 3
printf 'pb\nrr\nrrr\npa\n' > .ps_in
expect "rr and rrr undo each other" OK .ps_in 1 2 3
printf 'ra\nrra\nrra\nra\n' > .ps_in
expect "rotations that cancel out change nothing" KO .ps_in 3 1 2

for count in 3 4 5; do
	solved=0
	failed=0
	while IFS= read -r list; do
		# shellcheck disable=SC2206
		args=($list)
		"$PS_TOOL" solve "${args[@]}" > .ps_in
		ps_run_input .ps_in "${args[@]}"
		if [ "$(cat .ps_out)" != "OK" ]; then
			failed=$((failed + 1))
			[ $failed -le 2 ] && pc_detail "OK expected for ./checker $list with $(paste -sd ' ' .ps_in | head -c 60), got: $(head -c 20 .ps_out | head -1)"
		fi
		if [ -s .ps_in ]; then
			sed '$d' .ps_in > .ps_in2
			ps_run_input .ps_in2 "${args[@]}"
			if [ "$(cat .ps_out)" != "KO" ]; then
				failed=$((failed + 1))
				[ $failed -le 2 ] && pc_detail "KO expected for ./checker $list with an unfinished solution, got: $(head -c 20 .ps_out | head -1)"
			fi
		fi
		solved=$((solved + 1))
	done < <("$PS_TOOL" perms "$count")
	if [ $solved -eq 0 ]; then
		pc_bad "the test helper produced no list of $count numbers"
	elif [ $failed -eq 0 ]; then
		pc_ok "solutions for all $solved orders of $count numbers: OK, and KO once their last instruction is dropped"
	else
		pc_bad "$failed wrong answer(s) over the orders of $count numbers"
	fi
done
rm -f .ps_in .ps_in2
exit $pc_error
