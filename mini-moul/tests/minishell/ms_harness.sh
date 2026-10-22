#!/bin/bash

# Shared by the minishell check scripts. Sourced with the mini-moul
# directory as cwd; the student's project is at ../
#
# A case is a list of input lines. They are fed on stdin to bash and to
# ../minishell, each run in the same freshly built sandbox directory with
# the same environment, and the results are compared: stdout, the files the
# commands left in the sandbox, and whether anything went to stderr (the
# wording of an error message is up to you). Fed from a pipe, a readline
# shell still prints its prompt and echoes each line; ms_filter.pl learns
# both from a calibration run and removes them. bash is the reference, as
# the subject says.

source ./utils/program_check.sh

MS_SANDBOX="$PWD/.ms_sandbox"
MS_CAL=.ms_cal
MS_TIMEOUT=${MS_TIMEOUT:-5}
MS_BASH="$(command -v bash)"
MS_BIN=""
MS_POST=""
MS_CHECK_CODE=0
MS_STDERR=1
MS_PATH="/usr/bin:/bin"

# ms_prepare [make target]: builds ../minishell if needed and calibrates
ms_prepare()
{
	pc_ensure_built .. minishell "$1" || exit 1
	MS_BIN="$(cd .. && pwd)/minishell"
	ms_calibrate
}

ms_fixture()
{
	chmod -R u+rwx "$MS_SANDBOX" 2> /dev/null
	rm -rf "$MS_SANDBOX"
	mkdir -p "$MS_SANDBOX/dir"
	(
		cd "$MS_SANDBOX" || exit 1
		printf 'alpha\n' > a.txt
		printf 'beta\ngamma\n' > b.txt
		: > empty
		printf 'x\n' > "a b.txt"
		printf 'hidden\n' > .hidden
		printf 'inner\n' > dir/inner.txt
		printf '#!/bin/sh\necho script ran\n' > script.sh
		chmod +x script.sh
		printf '#!/bin/sh\necho mytool ran\n' > dir/mytool
		chmod +x dir/mytool
		: > noperm
		chmod 000 noperm
	)
}

# ms_env_args <extra VAR=value...>: fills ms_envargs
ms_env_args()
{
	ms_envargs=("PATH=$MS_PATH" "HOME=$MS_SANDBOX" "PWD=$MS_SANDBOX" "TERM=dumb" "LC_ALL=C"
		"INPUTRC=/dev/null" "COLUMNS=10000" "LINES=100" "MS_A=hello" "MS_B=world" "MS_SPACE=  x   y  ")
	[ -n "$ASAN_OPTIONS" ] && ms_envargs+=("ASAN_OPTIONS=$ASAN_OPTIONS")
	[ -n "$LSAN_OPTIONS" ] && ms_envargs+=("LSAN_OPTIONS=$LSAN_OPTIONS")
	ms_envargs+=("$@")
}

# ms_exec <input file> <output prefix> <command...>
# Runs the command in the sandbox with the given stdin; <prefix>.out,
# .err and .code hold the results. A sanitizer report is passed on to
# stderr, where test.sh turns it into a Memory fail.
ms_exec()
{
	local input=$1 prefix=$2
	shift 2
	ms_env_args
	perl tests/minishell/ms_limited.pl "$MS_TIMEOUT" "$input" "$prefix.out" "$prefix.err" "$prefix.code" \
		"$MS_SANDBOX" env -i "${ms_envargs[@]}" "$@"
	if pc_sanitizer_report "$prefix.err"; then
		cat "$prefix.err" >&2
		exit 1
	fi
}

# ms_snapshot <file>: the sandbox's files and what is in them
ms_snapshot()
{
	(
		cd "$MS_SANDBOX" || exit 1
		find . | sort
		find . -type f | sort | while IFS= read -r f; do
			printf '== %s\n' "$f"
			cat "$f" 2> /dev/null
		done
	) > "$1" 2>&1
}

ms_calibrate()
{
	local dir=.ms_calibration
	[ -f "$MS_CAL" ] && return 0
	mkdir -p "$dir"
	ms_fixture
	printf 'true\n' > "$dir/in_true"
	printf ':\n' > "$dir/in_colon"
	: > "$dir/in_empty"
	printf 'exit\n' > "$dir/in_exit"
	printf 'cat << MSEOF\nMSL1\nMSEOF\n' > "$dir/in_here"
	for name in true empty exit here; do
		ms_exec "$dir/in_$name" "$dir/$name" "$MS_BIN"
	done
	perl tests/minishell/ms_filter.pl calibrate "$dir" > "$MS_CAL"
	rm -rf "$dir"
}

# ms_clean <stream out|err> <input file> <raw file>: what the commands printed
ms_clean()
{
	perl tests/minishell/ms_filter.pl clean "$MS_CAL" "$1" "$2" < "$3"
}

# ms_show <file>: one line, escapes visible, cut short
ms_show()
{
	perl -0pe 's/\n/\\n/g; s/\t/\\t/g; s/[^\x20-\x7e]/?/g; $_ = substr($_, 0, 90) . (length($_) > 90 ? "..." : "")' "$1"
}

ms_post()
{
	if [ -n "$MS_POST" ]; then
		"$MS_POST" < "$1" > "$1.post"
		mv "$1.post" "$1"
	fi
}

ms_post_env() { grep '^MS_' | sed 's/^declare -x //' | sort; }
ms_post_words() { tr ' ' '\n' | sort; }

# ms_run_case <binary> <input file>: runs bash then the shell on the input;
# results in .ms_ref.* (bash) and .ms_got.* (the shell, prompts removed).
ms_run_case()
{
	local bin=$1 input=$2
	ms_fixture
	ms_exec "$input" .ms_ref "$MS_BASH" --noprofile --norc
	ms_snapshot .ms_ref.fs
	ms_fixture
	ms_exec "$input" .ms_got_raw "$bin"
	ms_snapshot .ms_got.fs
	ms_clean out "$input" .ms_got_raw.out > .ms_got.out
	ms_clean err "$input" .ms_got_raw.err > .ms_got.err
	read -r ms_ref_code _ _ < .ms_ref.code
	read -r ms_got_code ms_got_sig ms_got_to < .ms_got_raw.code
	ms_post .ms_ref.out
	ms_post .ms_got.out
}

# ms_case <label> <line>...
# Compares the shell with bash on these input lines.
ms_case()
{
	local label=$1
	shift
	printf '%s\n' "$@" > .ms_in
	ms_run_case "${MS_RUN_BIN:-$MS_BIN}" .ms_in
	ms_judge "$label" "$*"
}

ms_judge()
{
	local label=$1 script=$2 problem=""
	if [ "$ms_got_to" -eq 1 ]; then
		problem="no answer after ${MS_TIMEOUT}s (does it stop at the end of its input?)"
	elif [ "$ms_got_sig" -ne 0 ]; then
		problem="crashed (signal $ms_got_sig)"
	elif ! cmp -s .ms_ref.out .ms_got.out; then
		problem="stdout differs from bash"
	elif ! cmp -s .ms_ref.fs .ms_got.fs; then
		problem="the files left behind differ from bash"
	elif [ "$MS_STDERR" -eq 1 ] && [ -s .ms_ref.err ] && [ ! -s .ms_got.err ]; then
		problem="bash writes an error on stderr, the shell writes nothing there"
	elif [ "$MS_STDERR" -eq 1 ] && [ ! -s .ms_ref.err ] && [ -s .ms_got.err ]; then
		problem="the shell writes on stderr, bash does not"
	elif [ "$MS_CHECK_CODE" -eq 1 ] && [ "$ms_ref_code" != "$ms_got_code" ]; then
		problem="exit status $ms_got_code, bash gives $ms_ref_code"
	fi
	if [ -z "$problem" ]; then
		pc_ok "$label"
		return
	fi
	pc_bad "$label: $problem"
	pc_detail "input:    $(printf '%s' "$script" | perl -pe 's/\n/ ; /g' | cut -c1-90)"
	case "$problem" in
		"stdout differs"*)
			pc_detail "bash:     $(ms_show .ms_ref.out)"
			pc_detail "minishell: $(ms_show .ms_got.out)"
			;;
		"the files"*)
			pc_detail "bash:     $(head -c 200 .ms_ref.fs | tr '\n' ' ' | cut -c1-90)"
			pc_detail "minishell: $(head -c 200 .ms_got.fs | tr '\n' ' ' | cut -c1-90)"
			;;
		*stderr*)
			pc_detail "stderr:   $(ms_show .ms_got.err)"
			;;
	esac
}

# ms_case_expect <label> <expected stdout, printf format> <line>...
# Runs the shell alone and compares its stdout with a fixed text, for
# behaviour bash cannot show in a script (it stops at a syntax error).
ms_case_expect()
{
	local label=$1 expected=$2
	shift 2
	printf '%s\n' "$@" > .ms_in
	printf "$expected" > .ms_ref.out
	ms_fixture
	ms_exec .ms_in .ms_got_raw "${MS_RUN_BIN:-$MS_BIN}"
	ms_clean out .ms_in .ms_got_raw.out > .ms_got.out
	read -r ms_got_code ms_got_sig ms_got_to < .ms_got_raw.code
	if [ "$ms_got_to" -eq 1 ]; then
		pc_bad "$label: no answer after ${MS_TIMEOUT}s (does it stop at the end of its input?)"
	elif [ "$ms_got_sig" -ne 0 ]; then
		pc_bad "$label: crashed (signal $ms_got_sig)"
	elif cmp -s .ms_ref.out .ms_got.out; then
		pc_ok "$label"
	else
		pc_bad "$label: stdout is not what bash prints"
		pc_detail "input:    $(printf '%s' "$*" | cut -c1-90)"
		pc_detail "expected: $(ms_show .ms_ref.out)"
		pc_detail "got:      $(ms_show .ms_got.out)"
	fi
}

# ms_run_only <label> <line>...
# Runs the shell alone on the lines: it must stop by itself without
# crashing (a sanitizer report ends the script as a Memory fail).
ms_run_only()
{
	local label=$1
	shift
	printf '%s\n' "$@" > .ms_in
	ms_fixture
	ms_exec .ms_in .ms_got_raw "${MS_RUN_BIN:-$MS_BIN}"
	read -r ms_got_code ms_got_sig ms_got_to < .ms_got_raw.code
	if [ "$ms_got_to" -eq 1 ]; then
		pc_bad "$label: no answer after ${MS_TIMEOUT}s (does it stop at the end of its input?)"
	elif [ "$ms_got_sig" -ne 0 ]; then
		pc_bad "$label: crashed (signal $ms_got_sig)"
	else
		pc_ok "$label: no memory error"
	fi
}

# ms_sanitized: MS_SAN_BIN is a copy of the project built with
# AddressSanitizer; returns 1 (after a [!]) when there is none
ms_sanitized()
{
	if pc_instrumented_build asan_ms .. "-fsanitize=address -g" __asan_init minishell all; then
		MS_SAN_BIN="$PWD/.instr_asan_ms/$(basename "$(cd .. && pwd)")/minishell"
		return 0
	fi
	pc_warn "could not build minishell with AddressSanitizer (does your Makefile hardcode its compiler?); this check is skipped"
	return 1
}

# Leaks inside readline and ncurses are not yours to fix
ms_lsan_suppressions()
{
	printf 'leak:libreadline\nleak:libhistory\nleak:libtinfo\nleak:libncurses\nleak:libncursesw\nleak:libtermcap\n' > .lsan.supp
	LSAN_OPTIONS="suppressions=$PWD/.lsan.supp"
}

# ms_prepare_bonus: like ms_prepare, after make bonus when the Makefile has
# that rule (the bonus may be built into the same minishell)
ms_prepare_bonus()
{
	if make -C .. -n bonus > /dev/null 2>&1; then
		make -C .. bonus > .make_out 2>&1
		rm -f .make_out "$MS_CAL"
	fi
	ms_prepare
}
