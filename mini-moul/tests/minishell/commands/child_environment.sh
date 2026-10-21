#!/bin/bash

# What child processes receive: the exported variables and the working directory.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "a variable from the environment reaches a child" 'bash -c '"'"'echo $MS_A'"'"''
ms_case "an exported variable reaches a child" 'export MS_N=5' 'bash -c '"'"'echo $MS_N'"'"''
ms_case "a changed variable reaches a child" 'export MS_A=changed' 'bash -c '"'"'echo $MS_A'"'"''
ms_case "an unset variable does not reach a child" 'unset MS_A' 'bash -c '"'"'echo [$MS_A]'"'"''
ms_case "a child sees the new directory" 'cd dir' 'bash -c '"'"'pwd'"'"''
ms_case "env as a child process" 'env | grep ^MS_A='
ms_case "a child that prints on stderr" 'ls nonexistent_file'
ms_case "a child with an exit status" 'sh -c '"'"'exit 7'"'"'' 'echo $?'
ms_case "a script keeps its own arguments" './script.sh a b c'

exit $pc_error
