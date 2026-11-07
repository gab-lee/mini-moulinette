#!/bin/bash

# Exercise 1: ft_archive_creation adds # at the end of each line and can save the result.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_archivist/pa_harness.sh

file=../ex1/ft_archive_creation.py
pa_check_file "$file" len,open,print,input sys,typing --no-with
pa_fresh
pa_run "$file" "\n" "no new file name" ancient_fragment.txt
pa_has out "the transformed content is shown" 'established 2087#$' 'entropy wars#$' 'against oblivion#$'
pa_no_new_file "nothing is saved when the name is empty" new_fragment.txt
[ "$(ls "$PA_WORK" | wc -l | tr -d ' ')" -eq 1 ] && pc_ok "no file is created" || pc_bad "no file should be created when the name is empty"
pa_compare "no new file name" "=== Cyber Archives Recovery & Preservation ===\nAccessing file 'ancient_fragment.txt'\n---\n\n[FRAGMENT 001] Digital preservation protocols established 2087\n[FRAGMENT 002] Knowledge must survive the entropy wars\n[FRAGMENT 003] Every byte saved is a victory against oblivion\n\n---\nFile 'ancient_fragment.txt' closed.\n\nTransform data:\n---\n\n[FRAGMENT 001] Digital preservation protocols established 2087#\n[FRAGMENT 002] Knowledge must survive the entropy wars#\n[FRAGMENT 003] Every byte saved is a victory against oblivion#\n\n---\nEnter new file name (or empty): Not saving data.\n"
pa_fresh
pa_run "$file" "new_fragment.txt\n" "saving to new_fragment.txt" ancient_fragment.txt
pa_file_is "new_fragment.txt holds every line followed by #" new_fragment.txt '[FRAGMENT 001] Digital preservation protocols established 2087#\n[FRAGMENT 002] Knowledge must survive the entropy wars#\n[FRAGMENT 003] Every byte saved is a victory against oblivion#\n'
pa_has out "the saved file name is shown" 'new_fragment\.txt'
pa_file_is "the original file is untouched" ancient_fragment.txt "$PA_CONTENT"
pa_fresh
printf 'old content\n' > "$PA_WORK/new_fragment.txt"
pa_run "$file" "new_fragment.txt\n" "an existing file is replaced" ancient_fragment.txt
pa_file_is "an existing file is replaced, not appended to" new_fragment.txt '[FRAGMENT 001] Digital preservation protocols established 2087#\n[FRAGMENT 002] Knowledge must survive the entropy wars#\n[FRAGMENT 003] Every byte saved is a victory against oblivion#\n'
pa_run "$file" "" "nonexistent source file" foo
pa_has any "nonexistent source file: the error is reported, not raised" 'foo' 'No such file or directory'
pa_run "$file" "" "no argument"
pa_has any "no argument: a usage message" '[Uu]sage'
pa_clean
exit $pc_error
