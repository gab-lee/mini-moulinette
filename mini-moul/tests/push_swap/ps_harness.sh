#!/bin/bash

# Shared by the push_swap check scripts. Sourced with the mini-moul
# directory as cwd; the student's project is at ../
#
# The checks run ../push_swap (or ../checker for the bonus). When the
# student's Makefile lets us override CC, they run a copy built with
# AddressSanitizer instead, so a leak or an overflow is a Memory fail.
# ps_tool.c (a simulator and an input generator) is compiled once per run.

source ./utils/program_check.sh

PS_TOOL=./.ps_tool
PS_TIMEOUT=${PS_TIMEOUT:-20}
PS_BIN=""
ps_code=0
ps_ops=0
ps_problem=""

ps_build_tool()
{
	[ -x "$PS_TOOL" ] && return 0
	cc -Wall -Wextra -Werror -O2 -o "$PS_TOOL" tests/push_swap/ps_tool.c
}

# ps_prepare <push_swap|checker>
# Builds the program with the student's Makefile if needed, then picks the
# binary the checks will run.
ps_prepare()
{
	local program=$1 targets="all" kind=asan project
	if [ "$program" = "checker" ]; then
		targets="all bonus"
		kind=asan_bonus
	fi
	ps_build_tool || { printf "    ${RED}cannot build the push_swap test helper${DEFAULT}\n"; exit 1; }
	if [ "$program" = "checker" ]; then
		pc_ensure_built .. checker bonus || exit 1
	else
		pc_ensure_built .. push_swap || exit 1
	fi
	project="$(basename "$(cd .. && pwd)")"
	# shellcheck disable=SC2086
	if pc_instrumented_build "$kind" .. "-fsanitize=address -g" __asan_init "$program" $targets; then
		PS_BIN=".instr_$kind/$project/$program"
	else
		PS_BIN="../$program"
		if [ ! -f ".instr_$kind/warned" ]; then
			pc_warn "could not build $program with AddressSanitizer (does your Makefile hardcode its compiler?); memory errors are not checked"
			touch ".instr_$kind/warned"
		fi
	fi
}

# ps_run <args...>
# Runs the program with a time limit; stdout in .ps_out, stderr in .ps_err,
# exit status in ps_code (pc_timeout and pc_signal tell a timeout or a crash
# from it). A sanitizer report is
# passed on to stderr, where test.sh turns it into a Memory fail.
ps_run()
{
	pc_limited "$PS_TIMEOUT" "$PS_BIN" "$@" > .ps_out 2> .ps_err < /dev/null
	ps_code=$pc_exit
	if pc_sanitizer_report .ps_err; then
		cat .ps_err >&2
		exit 1
	fi
}

# ps_run_input <input file> <args...>: same, with the file as stdin
ps_run_input()
{
	local input=$1
	shift
	pc_limited "$PS_TIMEOUT" "$PS_BIN" "$@" > .ps_out 2> .ps_err < "$input"
	ps_code=$pc_exit
	if pc_sanitizer_report .ps_err; then
		cat .ps_err >&2
		exit 1
	fi
}

ps_crash_text()
{
	if [ "$pc_timeout" -eq 1 ]; then
		printf 'no answer after %ss' "$PS_TIMEOUT"
	elif [ "$pc_signal" -ne 0 ]; then
		printf 'crashed (signal %s)' "$pc_signal"
	fi
}

# ps_sort_case <args...>
# Runs push_swap and replays its instructions. ps_problem is empty when the
# stack ends sorted, else says why not; ps_ops is the instruction count.
ps_sort_case()
{
	local result
	ps_ops=0
	ps_problem=""
	ps_run "$@"
	ps_problem="$(ps_crash_text)"
	[ -n "$ps_problem" ] && return
	if [ -s .ps_err ]; then
		ps_problem="wrote on stderr: $(head -c 60 .ps_err | head -1)"
		return
	fi
	result="$("$PS_TOOL" sim "$@" < .ps_out)"
	case "$result" in
		OK*) ps_ops=${result#OK } ;;
		KO*) ps_ops=${result#KO }; ps_problem="the instructions do not leave stack a sorted with stack b empty" ;;
		*) ps_problem="${result#BAD }" ;;
	esac
}

# ps_points <100|500> <operations>: points on the evaluation scale (0 to 5)
ps_points()
{
	local limits points=5 limit
	case "$1" in
		100) limits="700 900 1100 1300 1500" ;;
		*) limits="5500 7000 8500 10000 11500" ;;
	esac
	for limit in $limits; do
		if [ "$2" -lt "$limit" ]; then
			printf '%s' "$points"
			return
		fi
		points=$((points - 1))
	done
	printf '0'
}

# ps_every_permutation <count> <max operations>
# Sorts every permutation of count numbers; one line for the whole set.
ps_every_permutation()
{
	local count=$1 max=$2 list worst=0 bad=0 total=0 shown=0
	local args
	while IFS= read -r list; do
		# shellcheck disable=SC2206
		args=($list)
		total=$((total + 1))
		ps_sort_case "${args[@]}"
		if [ -z "$ps_problem" ] && [ "$ps_ops" -gt "$max" ]; then
			ps_problem="$ps_ops operations (at most $max allowed)"
		fi
		if [ -n "$ps_problem" ]; then
			bad=$((bad + 1))
			if [ $shown -lt 3 ]; then
				pc_detail "./push_swap $list: $ps_problem"
				shown=$((shown + 1))
			fi
		elif [ "$ps_ops" -gt "$worst" ]; then
			worst=$ps_ops
		fi
	done < <("$PS_TOOL" perms "$count")
	if [ $total -eq 0 ]; then
		pc_bad "the test helper produced no list of $count numbers"
	elif [ $bad -eq 0 ]; then
		pc_ok "all $total orders of $count numbers are sorted, at most $worst operations (limit $max)"
	else
		pc_bad "$bad of $total orders of $count numbers fail"
	fi
}

# ps_random_sorts <count> <fail limit> <first seed> <number of lists> [extremes]
# Sorts random lists; fails on a bad answer or beyond the lowest point on
# the evaluation scale, and warns below full marks.
ps_random_sorts()
{
	local count=$1 limit=$2 seed=$3 lists=$4 extremes=$5 worst=0 i failed=0
	local list args points
	i=0
	while [ $i -lt "$lists" ]; do
		list="$("$PS_TOOL" gen "$count" $((seed + i)) $extremes)"
		if [ -z "$list" ]; then
			pc_bad "the test helper produced no list of $count numbers"
			return
		fi
		# shellcheck disable=SC2206
		args=($list)
		ps_sort_case "${args[@]}"
		if [ -n "$ps_problem" ]; then
			pc_bad "$count random numbers (seed $((seed + i))): $ps_problem"
			failed=1
		else
			[ "$ps_ops" -gt "$worst" ] && worst=$ps_ops
		fi
		i=$((i + 1))
	done
	[ $failed -eq 1 ] && return
	if [ "$worst" -ge "$limit" ]; then
		pc_bad "$count random numbers: $worst operations in the worst of $lists lists (must be under $limit)"
		return
	fi
	pc_ok "$count random numbers: sorted, $worst operations in the worst of $lists lists"
	[ "$count" -eq 100 ] || [ "$count" -eq 500 ] || return 0
	points="$(ps_points "$count" "$worst")"
	if [ "$points" -lt 5 ]; then
		pc_warn "$worst operations is $points/5 on the evaluation scale; under $([ "$count" -eq 100 ] && echo 700 || echo 5500) earns 5/5"
	fi
}
