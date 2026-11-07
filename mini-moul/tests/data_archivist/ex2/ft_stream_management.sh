#!/bin/bash

# Exercise 2: ft_stream_management sends errors to stderr and reads without input().
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_archivist/pa_harness.sh

file=../ex2/ft_stream_management.py
pa_check_file "$file" len,open,print sys,typing --no-with
pa_fresh
pa_run "$file" "" "nonexistent file" foo
pa_has err "nonexistent file: the error goes to stderr" 'foo' 'No such file or directory'
pa_lacks out "nonexistent file: the error is not on stdout" 'No such file or directory'
pa_hint err "nonexistent file: the error message has no clear [STDERR] prefix" '\[STDERR\]'
pa_has out "nonexistent file: the file is announced on stdout" "foo"
pa_compare "nonexistent file" "=== Cyber Archives Recovery & Preservation ===\nAccessing file 'foo'\n"
pa_fresh
mkdir "$PA_WORK/vault"
pa_run "$file" "vault\n" "saving to a directory" ancient_fragment.txt
pa_has err "saving to a directory: the error goes to stderr" 'vault' 'Is a directory'
pa_has out "saving to a directory: the data is reported as not saved" '[Nn]ot saved'
pa_file_is "the original file is untouched" ancient_fragment.txt "$PA_CONTENT"
pa_run "$file" "new_fragment.txt\n" "saving to new_fragment.txt" ancient_fragment.txt
pa_file_is "new_fragment.txt holds every line followed by #" new_fragment.txt '[FRAGMENT 001] Digital preservation protocols established 2087#\n[FRAGMENT 002] Knowledge must survive the entropy wars#\n[FRAGMENT 003] Every byte saved is a victory against oblivion#\n'
pa_has out "the saved file name is shown" 'new_fragment\.txt'
pa_fresh
pa_run "$file" "" "end of input instead of a name" ancient_fragment.txt
pa_has out "the transformed content is shown" 'established 2087#$' 'against oblivion#$'
[ "$(ls "$PA_WORK" | wc -l | tr -d ' ')" -eq 1 ] && pc_ok "nothing is saved when the input ends" || pc_bad "no file should be created when the input is empty"
pa_run "$file" "" "no argument"
pa_has any "no argument: a usage message" '[Uu]sage'
pa_clean
exit $pc_error
