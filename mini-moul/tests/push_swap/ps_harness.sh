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
	local numbers=() arg
	for arg in "$@"; do
		case "$arg" in
			--[a-z]*) ;;
			*) numbers+=("$arg") ;;
		esac
	done
	result="$("$PS_TOOL" sim "${numbers[@]}" < .ps_out)"
	case "$result" in
		OK*) ps_ops=${result#OK } ;;
		KO*) ps_ops=${result#KO }; ps_problem="the instructions do not leave stack a sorted with stack b empty" ;;
		*) ps_problem="${result#BAD }" ;;
	esac
}

# ps_tier <100|500> <operations>: pass, good or excellent (subject VI.6),
# or fail when the count is at or above the pass limit
ps_tier()
{
	local pass good excellent
	case "$1" in
		100) pass=2000; good=1500; excellent=700 ;;
		*) pass=12000; good=8000; excellent=5500 ;;
	esac
	if [ "$2" -lt "$excellent" ]; then
		printf 'excellent'
	elif [ "$2" -lt "$good" ]; then
		printf 'good'
	elif [ "$2" -lt "$pass" ]; then
		printf 'pass'
	else
		printf 'fail'
	fi
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

# ps_random_sorts <count> <seed> <number of lists> [extremes] -- <push_swap options...>
# Sorts random lists; fails on a bad answer or at the subject's pass limit
# (2000 for 100 numbers, 12000 for 500), and warns below the excellent tier
# (700 / 5500). Options such as --simple go before the numbers.
ps_random_sorts()
{
	local count=$1 seed=$2 lists=$3 extremes="" worst=0 i failed=0
	local list args tier limit excellent
	shift 3
	[ "$1" = "extremes" ] && { extremes=extremes; shift; }
	[ "$1" = "--" ] && shift
	case "$count" in
		100) limit=2000; excellent=700 ;;
		500) limit=12000; excellent=5500 ;;
		*) limit=1000000000; excellent=0 ;;
	esac
	i=0
	while [ $i -lt "$lists" ]; do
		list="$("$PS_TOOL" gen "$count" $((seed + i)) $extremes)"
		if [ -z "$list" ]; then
			pc_bad "the test helper produced no list of $count numbers"
			return
		fi
		# shellcheck disable=SC2206
		args=($list)
		ps_sort_case "$@" "${args[@]}"
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
	tier="$(ps_tier "$count" "$worst")"
	if [ "$tier" != "excellent" ]; then
		pc_warn "$worst operations is \"$tier\" in the subject's benchmark; under $excellent is excellent"
	fi
}
