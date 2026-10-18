#!/bin/bash

# unset, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "an unset variable expands to nothing" 'unset MS_A' 'echo $MS_A'
ms_case "unset two variables" 'unset MS_A MS_B' 'echo [$MS_A$MS_B]'
ms_case "unset a variable that does not exist" 'unset NOT_SET' 'echo $?'
ms_case "unset with no argument" 'unset' 'echo $?'
ms_case "an identifier cannot start with a digit" 'unset 1BAD' 'echo $?'
ms_case "a variable can be set again after unset" 'unset MS_A' 'export MS_A=back' 'echo $MS_A'
ms_case "unset removes the variable from the environment" 'unset MS_A' 'bash -c '"'"'echo [$MS_A]'"'"''
MS_POST=ms_post_env
ms_case "env does not list an unset variable" 'unset MS_A MS_B' 'env'

exit $pc_error
