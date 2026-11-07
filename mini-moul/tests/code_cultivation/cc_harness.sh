#!/bin/bash

# Shared by the code_cultivation check scripts ("Code Cultivation", object
# oriented Python, subject v3.2). Sourced with the mini-moul directory as
# cwd; the student's exercises are at ../ex0/ ... ../ex6/.
#
# The subject leaves the design free and says the output is not checked
# strictly, so the checks are structural (classes, methods, protected
# attributes, inheritance, type hints, flake8 and mypy), behavioural where
# the subject names the behaviour (cc_dynamic.py imports the file and tries
# Plant), and the script must run without error. A difference from the
# subject's example output is only a [!].

source ./utils/py_check.sh

CC_TIMEOUT=${CC_TIMEOUT:-10}
CC_DIR=./tests/code_cultivation

# cc_check_file <file> <allowed calls, comma separated> [utils/py_ast.py options...]
# Exits when the file does not exist.
cc_check_file()
{
	local file=$1 allow=$2
	shift 2
	py_need_python
	if [ ! -f "$file" ]; then
		pc_bad "${file#../} is missing"
		exit 1
	fi
	pc_ok "${file#../} exists"
	py_flake8 "$file"
	py_mypy "$file"
	py_report < <(python3 -I ./utils/py_ast.py "$file" --allow "$allow" "$@")
}

# cc_dynamic <file> <ex1|ex2|...|instances N>: tries the Plant class
# (instances N: runs the script and counts the plants it creates)
cc_dynamic()
{
	py_report < <(python3 -I "$CC_DIR/cc_dynamic.py" "$@")
}

# cc_run <file> <expected output file>
# python3 file must run without error; its output is compared with the
# subject's example, a difference is only a [!].
cc_run()
{
	local file=$1 expected=$2 line
	pc_limited "$CC_TIMEOUT" python3 -I "$file" > .cc_out 2> .cc_err < /dev/null
	if [ "$pc_timeout" -eq 1 ]; then
		pc_bad "python3 ${file#../}: no end after ${CC_TIMEOUT}s"
	elif [ "$pc_exit" -ne 0 ]; then
		pc_bad "python3 ${file#../} fails (exit status $pc_exit):"
		tail -3 .cc_err | sed 's/^/        /'
	elif [ -s .cc_err ]; then
		pc_bad "python3 ${file#../} wrote on stderr: $(head -c 60 .cc_err | head -1)"
	elif [ ! -s .cc_out ]; then
		pc_bad "python3 ${file#../} prints nothing"
	else
		pc_ok "python3 ${file#../} runs without error"
		if cmp -s .cc_out "$expected"; then
			pc_ok "output is the subject's example"
		else
			line="$(diff .cc_out "$expected" | head -1)"
			pc_warn "output differs from the subject's example (not checked strictly by the subject; first difference at line ${line%%[acd]*})"
		fi
	fi
	rm -f .cc_out .cc_err
}
