#!/bin/bash

# Exercise 0: ft_ancient_text reads a file given on the command line and shows it like cat.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_archivist/pa_harness.sh

file=../ex0/ft_ancient_text.py
pa_check_file "$file" len,open,print sys,typing --no-with
pa_fresh
pa_run "$file" "" "no argument"
pa_has any "no argument: a usage message" '[Uu]sage' '<file>'
pa_run "$file" "" "nonexistent file" foo
pa_has any "nonexistent file: the error is reported, not raised" 'foo' 'No such file or directory' '[Ee]rror'
pa_run "$file" "" "a directory" "$(mkdir -p "$PA_WORK/vault" && echo vault)"
pa_has any "a directory: the error is reported, not raised" 'vault' 'Is a directory'
pa_run "$file" "" "the subject's file" ancient_fragment.txt
pa_has out "the subject's file: name and content shown" "ancient_fragment\.txt" \
	'^\[FRAGMENT 001\] Digital preservation protocols established 2087$' \
	'^\[FRAGMENT 002\] Knowledge must survive the entropy wars$' \
	'^\[FRAGMENT 003\] Every byte saved is a victory against oblivion$' '^---' '[Cc]losed'
pa_compare "the subject's file" "=== Cyber Archives Recovery ===\nAccessing file 'ancient_fragment.txt'\n---\n\n[FRAGMENT 001] Digital preservation protocols established 2087\n[FRAGMENT 002] Knowledge must survive the entropy wars\n[FRAGMENT 003] Every byte saved is a victory against oblivion\n\n---\nFile 'ancient_fragment.txt' closed.\n"
printf 'alpha\n\nbeta \xc3\xa9\n' > "$PA_WORK/other.txt"
pa_run "$file" "" "another file (empty line, accent)" other.txt
pa_has out "another file: content shown" '^alpha$' '^beta é$'
if [ "$(id -u)" -ne 0 ]; then
	printf 'secret\n' > "$PA_WORK/locked.txt"
	chmod 000 "$PA_WORK/locked.txt"
	pa_run "$file" "" "an inaccessible file" locked.txt
	pa_has any "an inaccessible file: the error is reported" 'locked\.txt' 'Permission denied'
fi
pa_clean
exit $pc_error
