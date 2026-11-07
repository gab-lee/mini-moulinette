#!/bin/bash

# Shared by the data_quest check scripts ("Data Quest", Python collections,
# subject v3.1). Sourced with the mini-moul directory as cwd; the student's
# exercises are at ../ex0/ ... ../ex6/.
#
# The scripts take command-line arguments or stdin and print text, and ex3,
# ex5 and ex6 are random. So:
#   pq_has / pq_count   values the subject's example must show (a FAIL when
#                       missing), whatever the wording around them
#   pq_compare          the whole example text; a difference is only a [!]
#   pq_validate         random output checked for consistency
#   pq_dynamic          imports the file and calls its functions
# Every program runs in its own directory (python3 ft_x.py, so sys.argv[0]
# is the file name) and must end without a traceback.

source ./utils/py_check.sh

PQ_TIMEOUT=${PQ_TIMEOUT:-15}
PQ_DIR=./tests/data_quest
pq_crashed=0

# pq_check_file <file> <allowed calls> <allowed imports> [utils/py_ast.py options...]
pq_check_file()
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
	py_report < <(python3 -I ./utils/py_ast.py "$file" --allow "$allow,int,float,str" \
		--imports "$imports" "$@")
}

# pq_run <file> <stdin (printf format)> <label> [argument...]
# Runs python3 <file> in its directory; sets pq_crashed when it fails.
pq_run()
{
	local file=$1 input=$2 label=$3 dir name
	shift 3
	dir="$(dirname "$file")"
	name="$(basename "$file")"
	printf "$input" > .pq_in
	pc_limited "$PQ_TIMEOUT" bash -c 'cd "$1" && shift && exec python3 -I "$@"' \
		_ "$dir" "$name" "$@" > .pq_out 2> .pq_err < .pq_in
	pq_crashed=1
	if [ "$pc_timeout" -eq 1 ]; then
		pc_bad "$label: no end after ${PQ_TIMEOUT}s"
	elif [ "$pc_exit" -ne 0 ] || grep -q '^Traceback' .pq_err; then
		pc_bad "$label: the program crashes (exit status $pc_exit):"
		tail -3 .pq_err | sed 's/^/        /'
	elif [ -s .pq_err ]; then
		pc_bad "$label: wrote on stderr: $(head -c 60 .pq_err | head -1)"
	elif [ ! -s .pq_out ]; then
		pc_bad "$label: prints nothing"
	else
		pq_crashed=0
	fi
}

# pq_has <label> <regex>...: every regex (grep -E) must match the output
pq_has()
{
	local label=$1 pattern missing=""
	shift
	[ $pq_crashed -eq 1 ] && return
	for pattern in "$@"; do
		grep -Eq -- "$pattern" .pq_out || missing="$missing [$pattern]"
	done
	if [ -z "$missing" ]; then
		pc_ok "$label"
	else
		pc_bad "$label: the output should show$missing"
		head -8 .pq_out | sed 's/^/        /'
	fi
}

# pq_count <label> <regex> <n>: the regex matches exactly n lines
pq_count()
{
	local count
	[ $pq_crashed -eq 1 ] && return
	count=$(grep -Ec -- "$2" .pq_out)
	if [ "$count" -eq "$3" ]; then
		pc_ok "$1"
	else
		pc_bad "$1 (found $count lines matching [$2], expected $3)"
	fi
}

# pq_compare <label> <expected (printf format)>: a difference is a [!]
pq_compare()
{
	local line
	[ $pq_crashed -eq 1 ] && return
	printf "$2" > .pq_expected
	if cmp -s .pq_out .pq_expected; then
		pc_ok "$1: output is the subject's example"
	else
		line="$(diff .pq_out .pq_expected | head -1)"
		pc_warn "$1: output differs from the subject's example (first difference at line ${line%%[acd]*})"
	fi
	rm -f .pq_expected
}

# pq_validate <mode> <label>: consistency of a random output (.pq_out)
pq_validate()
{
	[ $pq_crashed -eq 1 ] && return
	py_report < <(python3 -I "$PQ_DIR/pq_validate.py" "$1" .pq_out | sed "s/^\([A-Z]*\) /\1 $2: /")
}

# pq_dynamic <file> <ex2|ex3|ex5>
pq_dynamic()
{
	py_report < <(python3 -I "$PQ_DIR/pq_dynamic.py" "$1" "$2")
}

pq_clean()
{
	rm -f .pq_in .pq_out .pq_err .pq_expected
}
