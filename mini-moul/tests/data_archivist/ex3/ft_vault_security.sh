#!/bin/bash

# Exercise 3: ft_vault_security provides secure_archive(), which uses a with statement.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_archivist/pa_harness.sh

file=../ex3/ft_vault_security.py
pa_check_file "$file" open,print typing --def secure_archive --with
pa_dynamic "$file"
pa_fresh
pa_run "$file" "" "the demo program"
pa_count "the demo shows the failures" '\(False, ' 2
pa_count "the demo shows the successes" '\(True, ' 2
pa_has out "the demo reads the subject's file" 'FRAGMENT 001' 'oblivion'
pa_compare "the demo program" "=== Cyber Archives Security ===\n\nUsing 'secure_archive' to read from a nonexistent file:\n(False, \"[Errno 2] No such file or directory: '/not/existing/file'\")\n\nUsing 'secure_archive' to read from an inaccessible file:\n(False, \"[Errno 2] No such file or directory: '/etc/master.passwd'\")\n\nUsing 'secure_archive' to read from a regular file:\n(True, '[FRAGMENT 001] Digital preservation protocols established 2087\\\\n[FRAGMENT 002] Knowledge must survive the entropy wars\\\\n[FRAGMENT 003] Every byte saved is a victory against oblivion\\\\n')\n\nUsing 'secure_archive' to write previous content to a new file:\n(True, 'Content successfully written to file')\n"
pa_clean
exit $pc_error
