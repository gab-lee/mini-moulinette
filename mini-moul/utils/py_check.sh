#!/bin/bash

# Helpers shared by the Python suites (growing_code, code_cultivation, ...).
# Sourced by the suites' harnesses with the mini-moul directory as cwd and
# config.sh already sourced. Needs the numbered pc_* output of
# program_check.sh.
#
#   py_tool <name>        command for a Python tool (flake8, mypy), empty
#                         when it is not installed
#   py_need_python        exits when python3 is missing
#   py_flake8 <file>      flake8 result as a numbered line
#   py_mypy <file>        mypy result as a numbered line
#   py_report             reads "OK|BAD|WARN <text>" lines on stdin (what
#                         the suites' Python checkers print) as numbered lines

source ./utils/program_check.sh

export PYTHONDONTWRITEBYTECODE=1

py_tool()
{
	if command -v "$1" > /dev/null 2>&1; then
		printf '%s' "$1"
	elif python3 -c "import $1" > /dev/null 2>&1; then
		printf 'python3 -m %s' "$1"
	fi
}

py_need_python()
{
	if ! command -v python3 > /dev/null 2>&1; then
		printf "    ${RED}python3 is not installed, so there is nothing to run${DEFAULT}\n"
		exit 1
	fi
}

py_flake8()
{
	local cmd
	cmd="$(py_tool flake8)"
	if [ -z "$cmd" ]; then
		pc_warn "flake8 is not installed, so the style (flake8) is not checked"
	elif $cmd --isolated "$1" > .py_flake8 2>&1; then
		pc_ok "flake8 reports nothing"
	else
		pc_bad "flake8 reports problems:"
		head -5 .py_flake8 | sed 's/^/        /'
	fi
	rm -f .py_flake8
}

py_mypy()
{
	local cmd
	cmd="$(py_tool mypy)"
	if [ -z "$cmd" ]; then
		pc_warn "mypy is not installed, so the type hints are not checked"
	elif $cmd --cache-dir=.mypy_cache "$1" > .py_mypy 2>&1; then
		pc_ok "mypy reports no error"
	else
		pc_bad "mypy reports errors:"
		head -5 .py_mypy | sed 's/^/        /'
	fi
	rm -rf .py_mypy .mypy_cache
}

py_report()
{
	local line
	while IFS= read -r line; do
		case "$line" in
			OK\ *) pc_ok "${line#OK }" ;;
			BAD\ *) pc_bad "${line#BAD }" ;;
			WARN\ *) pc_warn "${line#WARN }" ;;
		esac
	done
}
