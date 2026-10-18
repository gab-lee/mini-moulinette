#!/bin/bash

# $VARIABLE expansion, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "a variable" 'echo $MS_A'
ms_case "a variable in double quotes" 'echo "$MS_A"'
ms_case "a variable in single quotes is not expanded" "echo '\$MS_A'"
ms_case "two variables side by side" 'echo $MS_A$MS_B'
ms_case "two variables with a dash between" 'echo $MS_A-$MS_B'
ms_case "text before a variable" 'echo prefix$MS_A'
ms_case "the name stops at the first character that is not a letter, digit or _" 'echo $MS_A.txt'
ms_case "a variable followed by a slash" 'echo $MS_A/x'
ms_case "a longer name is another variable" 'echo $MS_Ax'
ms_case "an unset variable is empty" 'echo [$NOT_SET]'
ms_case "an unset variable inside a word" 'echo a$NOT_SET b'
ms_case "an unset variable in double quotes" 'echo "[$NOT_SET]"'
ms_case "an unquoted value is split into words" 'echo [$MS_SPACE]'
ms_case "a quoted value is kept as is" 'echo "[$MS_SPACE]"'
ms_case "a dollar sign alone" 'echo $'
ms_case "a dollar sign alone in double quotes" 'echo "$"'
ms_case "a dollar sign before a space" 'echo $ MS_A'
ms_case "variables in double quotes with text" 'echo "a $MS_A b $MS_B c"'
ms_case "a variable before a punctuation mark in double quotes" 'echo "$MS_A!"'
ms_case "a variable used as the command" 'export MS_CMD=echo' '$MS_CMD hello'
ms_case "an empty variable before the command" '$NOT_SET echo hi'
ms_case "the PATH variable" 'echo $PATH'
ms_case "the HOME variable" 'echo $HOME'
ms_case "a variable set in the shell reaches a child process" 'export MS_N=5' 'bash -c '"'"'echo $MS_N'"'"''
ms_case "a variable in an argument of an external command" 'ls $MS_A'
ms_case "a variable in a redirection target" 'export MS_F=out.txt' 'echo hi > $MS_F' 'cat out.txt'

exit $pc_error
