#!/bin/bash

# Shared by the data_archivist check scripts ("Data Archivist", file
# handling in Python, subject v3.0). Sourced with the mini-moul directory
# as cwd; the student's exercises are at ../ex0/ ... ../ex3/.
#
# The subject lets you reword the messages "as long as the essential
# information is preserved", so the checks look for that information
# (file names, error texts, the saved file's exact content, the stream the
# message goes to) and a difference from the example text is only a [!].
# Each program runs in a scratch directory (.pa_work) that holds
# ancient_fragment.txt, so relative names work as in the subject's example.

source ./utils/py_check.sh

PA_TIMEOUT=${PA_TIMEOUT:-15}
PA_DIR=./tests/data_archivist
PA_WORK=.pa_work
pa_crashed=0
PA_CONTENT='[FRAGMENT 001] Digital preservation protocols established 2087\n[FRAGMENT 002] Knowledge must survive the entropy wars\n[FRAGMENT 003] Every byte saved is a victory against oblivion\n'

# pa_check_file <file> <allowed calls> <allowed imports> [utils/py_ast.py options...]
pa_check_file()
{
	local file=$1 allow=$2 imports=$3
	shift 3
	py_need_python
	if [ ! -f "$file" ]; then
		pc_bad "${file#../} is missing"
		exit 1
	fi
	pc_ok "${file#../} exists"
	py_flake8 "$file"
	py_mypy "$file"
	py_report < <(python3 -I ./utils/py_ast.py "$file" --allow "$allow,str,int,float,list,dict,set,tuple" \
		--imports "$imports" "$@")
}

# pa_fresh: an empty scratch directory with ancient_fragment.txt
pa_fresh()
{
	rm -rf "$PA_WORK"
	mkdir "$PA_WORK"
	printf "$PA_CONTENT" > "$PA_WORK/ancient_fragment.txt"
}

# pa_run <file> <stdin (printf format)> <label> [argument...]
# Runs python3 <file> in the scratch directory; sets pa_crashed on failure.
pa_run()
{
	local file=$1 input=$2 label=$3 absolute
	shift 3
	absolute="$(cd "$(dirname "$file")" && pwd)/$(basename "$file")"
	printf "$input" > .pa_in
	pc_limited "$PA_TIMEOUT" bash -c 'cd "$1" && shift && exec python3 -I "$@"' \
		_ "$PA_WORK" "$absolute" "$@" > .pa_out 2> .pa_err < .pa_in
	pa_crashed=1
	if [ "$pc_timeout" -eq 1 ]; then
		pc_bad "$label: no end after ${PA_TIMEOUT}s"
	elif [ "$pc_exit" -ne 0 ] || grep -q '^Traceback' .pa_err; then
		pc_bad "$label: the program crashes (exit status $pc_exit):"
		tail -3 .pa_err | sed 's/^/        /'
	else
		pa_crashed=0
	fi
}

# pa_has <out|err|any> <label> <regex>...: every regex (grep -E) must match
pa_has()
{
	local where=$1 label=$2 pattern missing="" file=.pa_out
	shift 2
	[ $pa_crashed -eq 1 ] && return
	[ "$where" = "err" ] && file=.pa_err
	[ "$where" = "any" ] && { cat .pa_out .pa_err > .pa_both; file=.pa_both; }
	for pattern in "$@"; do
		grep -Eq -- "$pattern" "$file" || missing="$missing [$pattern]"
	done
	if [ -z "$missing" ]; then
		pc_ok "$label"
	else
		pc_bad "$label: the $where output should show$missing"
		head -6 "$file" | sed 's/^/        /'
	fi
	rm -f .pa_both
}

# pa_lacks <out|err> <label> <regex>: the regex must not match
pa_lacks()
{
	local file=.pa_out
	[ $pa_crashed -eq 1 ] && return
	[ "$1" = "err" ] && file=.pa_err
	if grep -Eq -- "$3" "$file"; then
		pc_bad "$2"
		head -4 "$file" | sed 's/^/        /'
	else
		pc_ok "$2"
	fi
}

# pa_hint <out|err> <label> <regex>: only a [!] when the regex does not match
pa_hint()
{
	local file=.pa_out
	[ $pa_crashed -eq 1 ] && return
	[ "$1" = "err" ] && file=.pa_err
	grep -Eq -- "$3" "$file" || pc_warn "$2"
}

# pa_count <label> <regex> <minimum>: at least <minimum> lines match (stdout)
pa_count()
{
	local count
	[ $pa_crashed -eq 1 ] && return
	count=$(grep -Ec -- "$2" .pa_out)
	if [ "$count" -ge "$3" ]; then
		pc_ok "$1"
	else
		pc_bad "$1 (found $count lines matching [$2], expected at least $3)"
	fi
}

# pa_file_is <label> <path in the scratch directory> <expected content (printf format)>
pa_file_is()
{
	printf "$3" > .pa_expected
	if [ ! -f "$PA_WORK/$2" ]; then
		pc_bad "$1: the file '$2' was not created"
	elif cmp -s "$PA_WORK/$2" .pa_expected; then
		pc_ok "$1"
	else
		pc_bad "$1: '$2' does not hold the expected content:"
		head -c 300 "$PA_WORK/$2" | sed -n l | head -4 | sed 's/^/        /'
	fi
	rm -f .pa_expected
}

# pa_no_new_file <label> <name>: nothing was saved under that name
pa_no_new_file()
{
	if [ -e "$PA_WORK/$2" ]; then
		pc_bad "$1: '$2' should not have been created"
	else
		pc_ok "$1"
	fi
}

# pa_compare <label> <expected stdout (printf format)>: a [!] when it differs
pa_compare()
{
	local line
	[ $pa_crashed -eq 1 ] && return
	printf "$2" > .pa_expected
	if cmp -s .pa_out .pa_expected; then
		pc_ok "$1: output is the subject's example"
	else
		line="$(diff .pa_out .pa_expected | head -1)"
		pc_warn "$1: output differs from the subject's example (the subject lets you reword it; first difference at line ${line%%[acd]*})"
	fi
	rm -f .pa_expected
}

# pa_dynamic <file>: secure_archive() (exercise 3)
pa_dynamic()
{
	local absolute work
	absolute="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
	pa_fresh
	work="$(cd "$PA_WORK" && pwd)"
	py_report < <(python3 -I "$PA_DIR/pa_dynamic.py" "$absolute" "$work")
}

pa_clean()
{
	rm -rf "$PA_WORK" .pa_in .pa_out .pa_err .pa_expected .pa_both
}
