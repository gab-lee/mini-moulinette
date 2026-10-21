#!/bin/bash

# pwd prints the current directory, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "pwd" 'pwd'
ms_case "pwd twice" 'pwd' 'pwd'
ms_case "pwd in a pipe" 'pwd | cat'

exit $pc_error
