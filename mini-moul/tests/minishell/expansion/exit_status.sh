#!/bin/bash

# $? after builtins, commands, failures and pipelines, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "\$? starts at 0" 'echo $?'
ms_case "after true" 'true' 'echo $?'
ms_case "after false" 'false' 'echo $?'
ms_case "\$? twice: echo itself succeeds" 'false' 'echo $?' 'echo $?'
ms_case "after a command that is not found" 'nonexistent_command' 'echo $?'
ms_case "after a command with a path that does not exist" './nonexistent' 'echo $?'
ms_case "after an absolute path that does not exist" '/nonexistent/command' 'echo $?'
ms_case "after a file that is not executable" './a.txt' 'echo $?'
ms_case "after a directory used as a command" './dir' 'echo $?'
ms_case "after ls on a missing file" 'ls nonexistent_file' 'echo $?'
ms_case "after a successful external command" 'ls' 'echo $?'
MS_STDERR=0 ms_case "after a command killed by a signal" 'sh -c '"'"'kill -9 $$'"'"'' 'echo $?'
ms_case "after a child that exits with 3" 'sh -c '"'"'exit 3'"'"'' 'echo $?'
ms_case "after a failed redirection" 'cat < nonexistent_file' 'echo $?'
ms_case "after cd to a missing directory" 'cd nonexistent' 'echo $?'
ms_case "a pipeline gives the status of its last command (success)" 'false | true' 'echo $?'
ms_case "a pipeline gives the status of its last command (failure)" 'true | false' 'echo $?'
ms_case "after an empty command in quotes" '""' 'echo $?'
ms_case "\$? inside double quotes" 'false' 'echo "status: $?"'
ms_case "\$? next to text" 'false' 'echo $?x'

exit $pc_error
