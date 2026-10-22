#!/bin/bash

# A syntax error prints nothing on stdout, something on stderr, sets the
# status to 2 and leaves the shell running. bash stops its script at a syntax
# error, so each case here is a single line compared with bash, and the
# lines after it are checked against a fixed text.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
for entry in \
	"a pipe at the end|echo hi |" \
	"a pipe at the start|| echo hi" \
	"two pipes in a row|echo hi | | cat" \
	"a lone pipe|\|" \
	"> without a file name|echo hi >" \
	">> without a file name|echo hi >>" \
	"< without a file name|cat <" \
	"<< without a delimiter|cat <<" \
	"> followed by >|echo hi > > out.txt" \
	"> followed by a pipe|echo hi > | cat" \
	"< followed by a pipe|cat < | cat" \
	"a redirection alone at the end of a pipe|echo hi | >" ; do
	label="${entry%%|*}"
	line="${entry#*|}"
	[ "$line" = '\|' ] && line='|'
	ms_case "$label" "$line"
	ms_case_expect "$label: the shell keeps running" 'after\n' "$line" 'echo after'
	ms_case_expect "$label: the status is 2" '2\n' "$line" 'echo $?'
done

exit $pc_error
