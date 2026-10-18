#!/bin/bash

# && and ||, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare_bonus
ms_case "true && echo" 'true && echo yes'
ms_case "false && echo" 'false && echo no'
ms_case "false || echo" 'false || echo yes'
ms_case "true || echo" 'true || echo no'
ms_case "&& chain" 'true && true && echo ok'
ms_case "|| chain" 'false || false || echo ok'
ms_case "&& stops at the first failure" 'true && false && echo no'
ms_case "|| stops at the first success" 'false || true || echo no'
ms_case "|| then && go left to right" 'true || false && echo yes'
ms_case "&& then || go left to right" 'false && true || echo yes'
ms_case "a failing command then ||" 'ls nonexistent_file || echo failed'
ms_case "a successful command then &&" 'ls a.txt && echo found'
ms_case "&& without spaces" 'true&&echo yes'
ms_case "|| without spaces" 'false||echo yes'
ms_case "\$? after &&" 'true && false' 'echo $?'
ms_case "\$? after ||" 'false || true' 'echo $?'
ms_case "a pipe inside &&" 'echo hi | cat && echo ok'
ms_case "a pipe inside ||" 'false || echo hi | cat'
ms_case "a redirection inside &&" 'echo hi > out.txt && cat out.txt'
ms_case "a command not found then ||" 'nonexistent_command || echo fallback'
ms_case "a builtin then &&" 'cd dir && pwd'

exit $pc_error
