#!/bin/bash

# Shared by the growing_code check scripts ("Growing Code", Python
# fundamentals, subject v3.0). Sourced with the mini-moul directory as cwd;
# the student's exercises are at ../ex0/ ... ../ex7/.
#
# gc_check_file  runs flake8 (utils/py_check.sh), then gc_check.py (AST
#                checks: only the requested functions, only authorized
#                calls, ...).
# gc_case        calls a function with a given stdin and compares what it
#                prints, byte for byte. stdin is a pipe, so input() prints
#                its prompt and the typed text is not echoed.

source ./utils/py_check.sh

GC_TIMEOUT=${GC_TIMEOUT:-10}
GC_DIR=./tests/growing_code

# gc_check_file <file> <allowed calls, comma separated> <function>... [--typed] [--recursive <name>]
# Style (flake8) and structure of one submitted file. Exits when the file
# does not exist.
gc_check_file()
{
	local file=$1 allow=$2 options=()
	shift 2
	py_need_python
	if [ ! -f "$file" ]; then
		pc_bad "${file#../} is missing"
		exit 1
	fi
	pc_ok "${file#../} exists"
	py_flake8 "$file"
	while [ $# -gt 0 ]; do
		case "$1" in
			--typed) options+=(--typed) ;;
			--recursive) options+=(--recursive "$2"); shift ;;
			*) options+=(--function "$1") ;;
		esac
		shift
	done
	python3 -I "$GC_DIR/gc_check.py" "$file" --allow "$allow" "${options[@]}" | py_report
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
