#!/bin/bash

# export, with and without arguments, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
MS_POST=ms_post_env
ms_case "export alone lists the variables" 'export'
ms_case "export alone lists a new variable" 'export MS_NEW=1' 'export'
MS_POST=""
ms_case "an exported variable is expanded" 'export MS_N=5' 'echo $MS_N'
ms_case "export replaces a value" 'export MS_A=1' 'echo $MS_A'
ms_case "export with another variable in the value" 'export MS_A=$MS_B' 'echo $MS_A'
ms_case "export with a quoted value" 'export MS_Q="a  b"' 'echo "$MS_Q"'
ms_case "export several variables" 'export MS_ONE=1 MS_TWO=2' 'echo $MS_ONE $MS_TWO'
ms_case "export a value with an equal sign" 'export MS_X=a=b' 'echo $MS_X'
ms_case "an identifier cannot start with a digit" 'export 1BAD=x' 'echo $?'
ms_case "an identifier cannot be empty" 'export =x' 'echo $?'
ms_case "an identifier cannot hold a dash" 'export MS-BAD=1' 'echo $?'
ms_case "a valid variable is set even if the next one is invalid" 'export MS_OK=1 1BAD=2' 'echo $MS_OK'
ms_case "export succeeds with status 0" 'export MS_OK=1' 'echo $?'

exit $pc_error
