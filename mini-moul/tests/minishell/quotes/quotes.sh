#!/bin/bash

# Single and double quotes, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "double quotes keep the spaces" 'echo "hello   world"'
ms_case "single quotes keep the spaces" "echo 'hello   world'"
ms_case "a single quote inside double quotes" 'echo "it'"'"'s"'
ms_case "a double quote inside single quotes" "echo 'say \"hi\"'"
ms_case "quoted and unquoted parts form one word" 'echo "a"b"c"'
ms_case "single and double quoted parts form one word" "echo \"a\"'b'c"
ms_case "an empty double quoted argument" 'echo "" x'
ms_case "an empty single quoted argument" "echo '' x"
ms_case "two empty arguments" 'echo "" "" x'
ms_case "only empty arguments" "echo '' ''"
ms_case "several quoted arguments" 'echo "a b" c "d  e"'
ms_case "single quotes protect the dollar sign" "echo 'a\$MS_A'\"\$MS_A\""
ms_case "double quotes expand the dollar sign next to single quotes" "echo \"\$MS_A\"'\$MS_B'"
ms_case "single quotes inside double quotes are kept" "echo \"'\$MS_A'\""
ms_case "double quotes inside single quotes are kept" "echo '\"\$MS_A\"'"
ms_case "a pipe sign in quotes is text" 'echo "a | b"'
ms_case "redirection signs in quotes are text" 'echo "a > b < c >> d"'
ms_case "redirection signs in single quotes are text" "echo '<< a'"
ms_case "a semicolon in quotes is text" 'echo "a;b"'
ms_case "a star in quotes is text" 'echo "*" '"'"'*'"'"''
ms_case "a quoted command name" '"echo" hi'
ms_case "a single quoted command name" "'echo' hi"
ms_case "quotes inside a command name" 'ec"ho" hi'
ms_case "a quoted file name with a space" 'cat "a b.txt"'
ms_case "a quoted redirection target with a space" 'echo hi > "out file.txt"' 'cat "out file.txt"'
ms_case "a quoted argument with a space for an external command" 'ls "a b.txt"'
ms_case "quotes right after another word" 'echo hello"world"'
ms_case "a space only argument" 'echo "   "x'

exit $pc_error
