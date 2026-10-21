#!/bin/bash

# Parentheses set the priority of && and ||, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare_bonus
ms_case "a group" '(echo a && echo b)'
ms_case "a group changes the priority" '(false || true) && echo ok'
ms_case "without the group the result differs" 'false || true && echo ok'
ms_case "a group on the right" 'true && (echo b || echo c)'
ms_case "a failing group then ||" 'false && (echo x) || echo y'
ms_case "nested groups" '((true && false) || (false || echo deep))'
ms_case "a group runs in its own process: cd does not leak out" '(cd dir)' 'pwd'
ms_case "a group runs in its own process: export does not leak out" '(export MS_G=1)' 'echo [$MS_G]'
ms_case "\$? after a group" '(false)' 'echo $?'
ms_case "\$? after a successful group" '(true)' 'echo $?'
ms_case "a group with a pipe" '(echo hi | cat) && echo ok'
ms_case "a group with a redirection" '(echo hi) > out.txt' 'cat out.txt'
ms_case "a group with spaces inside" '(  echo spaced  )'
ms_case "two groups" '(echo a) && (echo b)'

exit $pc_error
