#!/bin/bash

# Shared by the garden_guardian check scripts ("Garden Guardian", exception
# handling, subject v3.0). Sourced with the mini-moul directory as cwd; the
# student's exercises are at ../ex0/ ... ../ex4/.
#
# gg_check_file  flake8, mypy, then utils/py_ast.py (structure: functions,
#                try/except/finally, authorized calls, type hints).
# gg_dynamic     gg_dynamic.py imports the file and calls the functions
#                and classes the subject names.
# gg_run         the script must run without crashing; a difference from
#                the subject's example output is only a [!].

source ./utils/py_check.sh

GG_TIMEOUT=${GG_TIMEOUT:-10}
GG_DIR=./tests/garden_guardian

# gg_check_file <file> <allowed calls> <mypy: check|warn> [utils/py_ast.py options...]
gg_check_file()
{
	local file=$1 allow=$2 mypy=$3
	shift 3
	py_need_python
	if [ ! -f "$file" ]; then
		pc_bad "${file#../} is missing"
		exit 1
	fi
	pc_ok "${file#../} exists"
	py_flake8 "$file"
	if [ "$mypy" = "warn" ]; then
		py_mypy "$file" warn
	else
		py_mypy "$file"
	fi
	py_report < <(python3 -I ./utils/py_ast.py "$file" --allow "$allow" "$@")
}

# gg_dynamic <file> <ex0|ex1|...>
gg_dynamic()
{
	py_report < <(python3 -I "$GG_DIR/gg_dynamic.py" "$1" "$2")
}

# gg_run <file> <expected output file>
gg_run()
{
	local file=$1 expected=$2 line
	pc_limited "$GG_TIMEOUT" python3 -I "$file" > .gg_out 2> .gg_err < /dev/null
	if [ "$pc_timeout" -eq 1 ]; then
		pc_bad "python3 ${file#../}: no end after ${GG_TIMEOUT}s"
	elif [ "$pc_exit" -ne 0 ]; then
		pc_bad "python3 ${file#../} crashes (exit status $pc_exit); the subject wants a program that never crashes:"
		tail -3 .gg_err | sed 's/^/        /'
	elif [ -s .gg_err ]; then
		pc_bad "python3 ${file#../} wrote on stderr: $(head -c 60 .gg_err | head -1)"
	elif [ ! -s .gg_out ]; then
		pc_bad "python3 ${file#../} prints nothing"
	else
		pc_ok "python3 ${file#../} runs without crashing"
		if cmp -s .gg_out "$expected"; then
			pc_ok "output is the subject's example"
		else
			line="$(diff .gg_out "$expected" | head -1)"
			pc_warn "output differs from the subject's example (first difference at line ${line%%[acd]*})"
		fi
	fi
	rm -f .gg_out .gg_err
}
