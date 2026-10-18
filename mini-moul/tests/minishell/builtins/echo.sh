#!/bin/bash

# echo with -n, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare

ms_case "one word" 'echo hello'
ms_case "several words" 'echo hello world'
ms_case "no argument" 'echo'
ms_case "-n" 'echo -n hello'
ms_case "-n and nothing to print" 'echo -n'
ms_case "-n twice" 'echo -n -n hello'
ms_case "-nnn" 'echo -nnn hello'
ms_case "-na is not an option" 'echo -na hello'
ms_case "a lone -" 'echo - n'
ms_case "-n after the text is printed" 'echo hello -n'
ms_case "extra spaces between arguments" 'echo a    b     c'
ms_case "-n then another command" 'echo -n hello' 'echo world'
ms_case "a long argument" "echo $(printf 'x%.0s' {1..300})"
exit $pc_error
