#!/bin/bash

# A redirection that fails: an error on stderr, the command does not run, the shell goes on.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "< from a missing file" 'cat < nonexistent_file'
ms_case "the shell goes on after a failed <" 'cat < nonexistent_file' 'echo after'
ms_case "a failed < sets the status to 1" 'cat < nonexistent_file' 'echo $?'
ms_case "the command does not run when < fails" 'echo hi > out.txt < nonexistent_file' 'cat out.txt'
ms_case "> to a directory" 'echo hi > dir'
ms_case "a failed > sets the status to 1" 'echo hi > dir' 'echo $?'
ms_case "> into a missing directory" 'echo hi > nonexistent_dir/out.txt'
ms_case "> into a missing directory sets the status to 1" 'echo hi > nonexistent_dir/out.txt' 'echo $?'
ms_case ">> to a directory" 'echo hi >> dir'
ms_case "< from a directory" 'cat < dir'
ms_case "< from a file without read permission" 'cat < noperm' 'echo $?'
ms_case "> to a file without write permission" 'echo hi > noperm' 'echo $?'
ms_case "a builtin with a failed redirection does not run" 'cd dir > nonexistent_dir/x' 'pwd'

exit $pc_error
