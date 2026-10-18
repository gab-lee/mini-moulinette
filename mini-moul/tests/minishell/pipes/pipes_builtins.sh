#!/bin/bash

# Builtins in a pipeline run in their own process: they do not change the shell.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "cd in a pipe does not change the directory" 'cd dir | cat' 'pwd'
ms_case "export in a pipe does not set the variable" 'export MS_P=1 | cat' 'echo [$MS_P]'
ms_case "unset in a pipe does not unset the variable" 'unset MS_A | cat' 'echo $MS_A'
ms_case "exit in a pipe does not stop the shell" 'exit 3 | cat' 'echo still here'
ms_case "echo in the middle of a pipe" 'cat a.txt | echo hi | cat'
ms_case "env in a pipe" 'env | grep ^MS_A='
ms_case "pwd in a pipe" 'pwd | cat'
ms_case "a builtin at the end of a pipe" 'cat a.txt | cd dir' 'pwd'

exit $pc_error
