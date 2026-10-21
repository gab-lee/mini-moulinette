#!/bin/bash

# The subject says not to interpret an unclosed quote: an error on stderr,
# nothing on stdout, status 2, and the shell keeps running.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
for entry in \
	"an unclosed double quote|echo \"unclosed" \
	"an unclosed single quote|echo 'unclosed" \
	"an unclosed quote after text|echo hello \"world" \
	"a quote opened inside a word|echo ab\"cd" \
	"a single quote inside double quotes left open|echo \"it's" ; do
	label="${entry%%|*}"
	line="${entry#*|}"
	ms_case "$label" "$line"
	ms_case_expect "$label: the shell keeps running" 'after\n' "$line" 'echo after'
	ms_case_expect "$label: the status is 2" '2\n' "$line" 'echo $?'
done

exit $pc_error
