#!/bin/bash

# Shared by the growing_code check scripts ("Growing Code", Python
# fundamentals, subject v3.0). Sourced with the mini-moul directory as cwd;
# the student's exercises are at ../ex0/ ... ../ex7/.
#
# gc_check_file  runs flake8, then gc_check.py (AST checks: only the
#                requested functions, only authorized calls, ...).
# gc_case        calls a function with a given stdin and compares what it
#                prints, byte for byte. stdin is a pipe, so input() prints
#                its prompt and the typed text is not echoed.

source ./utils/program_check.sh

GC_TIMEOUT=${GC_TIMEOUT:-10}
GC_DIR=./tests/growing_code
export PYTHONDONTWRITEBYTECODE=1

# gc_tool <name>: the command for a Python tool (flake8, mypy), empty if
# it is not installed
gc_tool()
{
	if command -v "$1" > /dev/null 2>&1; then
		printf '%s' "$1"
	elif python3 -c "import $1" > /dev/null 2>&1; then
		printf 'python3 -m %s' "$1"
	fi
}

# gc_need_python: the checks run the student's code with python3
gc_need_python()
{
	if ! command -v python3 > /dev/null 2>&1; then
		printf "    ${RED}python3 is not installed, so there is nothing to run${DEFAULT}\n"
		exit 1
	fi
}

# gc_check_file <file> <allowed calls, comma separated> <function>... [--typed] [--recursive <name>]
# Style (flake8) and structure of one submitted file. Exits when the file
# does not exist.
gc_check_file()
{
	local file=$1 allow=$2 line flake8_cmd options=() fn
	shift 2
	gc_need_python
	if [ ! -f "$file" ]; then
		pc_bad "${file#../} is missing"
		exit 1
	fi
	pc_ok "${file#../} exists"

	flake8_cmd="$(gc_tool flake8)"
	if [ -z "$flake8_cmd" ]; then
		pc_warn "flake8 is not installed, so the style (flake8) is not checked"
	elif $flake8_cmd --isolated "$file" > .gc_flake8 2>&1; then
		pc_ok "flake8 reports nothing"
	else
		pc_bad "flake8 reports problems:"
		head -5 .gc_flake8 | sed 's/^/        /'
	fi
	rm -f .gc_flake8

	while [ $# -gt 0 ]; do
		case "$1" in
			--typed) options+=(--typed) ;;
			--recursive) options+=(--recursive "$2"); shift ;;
			*) options+=(--function "$1") ;;
		esac
		shift
	done
	while IFS= read -r line; do
		case "$line" in
			OK\ *) pc_ok "${line#OK }" ;;
			BAD\ *) pc_bad "${line#BAD }" ;;
			WARN\ *) pc_warn "${line#WARN }" ;;
		esac
	done < <(python3 -I "$GC_DIR/gc_check.py" "$file" --allow "$allow" "${options[@]}")
}

# gc_show <file>: the start of a file with escapes, for failure messages
gc_show()
{
	head -c 300 "$1" | sed -n l | head -6 | sed 's/^/        /'
}

# gc_case <label> <file> <function> <stdin (printf format)> <expected output (printf format)> [json arguments]
gc_case()
{
	local label=$1 file=$2 fn=$3 input=$4 expected=$5 extra=()
	[ -n "$6" ] && extra=("$6")
	printf "$input" > .gc_in
	printf "$expected" > .gc_expected
	pc_limited "$GC_TIMEOUT" python3 -I "$GC_DIR/gc_call.py" "$file" "$fn" "${extra[@]}" \
		> .gc_out 2> .gc_err < .gc_in
	if [ "$pc_timeout" -eq 1 ]; then
		pc_bad "$label: no answer after ${GC_TIMEOUT}s (waiting for more input?)"
	elif [ "$pc_exit" -ne 0 ] && [ "$pc_exit" -ne 3 ]; then
		pc_bad "$label: the function failed:"
		tail -3 .gc_err | sed 's/^/        /'
	elif [ "$pc_exit" -eq 3 ]; then
		if cmp -s .gc_out .gc_expected; then
			pc_ok "$label"
			pc_warn "the function returns a value; the subject wants it to print directly ($(head -c 60 .gc_err | head -1))"
		else
			pc_bad "$label: wrong output"
		fi
	elif ! cmp -s .gc_out .gc_expected; then
		pc_bad "$label: wrong output"
		printf "    ${RED}    expected:${DEFAULT}\n"
		gc_show .gc_expected
		printf "    ${RED}    got:${DEFAULT}\n"
		gc_show .gc_out
	elif [ -s .gc_err ]; then
		pc_bad "$label: wrote on stderr: $(head -c 60 .gc_err | head -1)"
	else
		pc_ok "$label"
	fi
	rm -f .gc_in .gc_out .gc_err .gc_expected
}
