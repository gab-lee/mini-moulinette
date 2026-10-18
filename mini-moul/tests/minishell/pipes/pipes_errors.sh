#!/bin/bash

# A command that fails inside a pipeline: the others still run and the status is the last one's.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "the first command is not found" 'nonexistent_command | cat'
ms_case "the first command is not found, the last one runs" 'nonexistent_command | echo ok'
ms_case "the last command is not found" 'echo hi | nonexistent_command'
ms_case "the last command is not found sets the status to 127" 'echo hi | nonexistent_command' 'echo $?'
ms_case "the middle command is not found" 'echo hi | nonexistent_command | cat' 'echo $?'
ms_case "the first command fails, the last one succeeds" 'ls nonexistent_file | cat' 'echo $?'
ms_case "the shell goes on after a failed pipeline" 'nonexistent_command | cat' 'echo after'
ms_case "a failed redirection in a pipeline" 'cat < nonexistent_file | cat' 'echo $?'

exit $pc_error
