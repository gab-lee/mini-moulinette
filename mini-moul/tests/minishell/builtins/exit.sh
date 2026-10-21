#!/bin/bash

# exit and its status, compared with bash (stdout and exit status).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
MS_CHECK_CODE=1
ms_case "exit" 'exit'
ms_case "exit 0" 'exit 0'
ms_case "exit 1" 'exit 1'
ms_case "exit 42" 'exit 42'
ms_case "exit 255" 'exit 255'
ms_case "exit 256 wraps to 0" 'exit 256'
ms_case "exit 300 wraps to 44" 'exit 300'
ms_case "exit -1 gives 255" 'exit -1'
ms_case "exit with a quoted number" 'exit "5"'
ms_case "exit with extra spaces" 'exit   7   '
ms_case "exit with a word is an error" 'exit abc'
ms_case "exit with too many arguments does not exit" 'exit 1 2' 'echo after'
ms_case "exit uses the status of the last command" 'false' 'exit'
ms_case "exit after a successful command" 'true' 'exit'
ms_case "nothing runs after exit" 'echo before' 'exit 5' 'echo after'
ms_case "exit in a pipe does not stop the shell" 'exit 3 | cat' 'echo still here'

exit $pc_error
