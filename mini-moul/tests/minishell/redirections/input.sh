#!/bin/bash

# < compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "< feeds a file to a command" 'cat < a.txt'
ms_case "< without spaces" 'cat <a.txt'
ms_case "< with no space before it" 'cat<a.txt'
ms_case "< before the command" '< a.txt cat'
ms_case "the last < wins" 'cat < a.txt < b.txt'
ms_case "< with wc" 'wc -l < b.txt'
ms_case "< with grep" 'grep alpha < a.txt'
ms_case "< from an empty file" 'cat < empty'
ms_case "< and > together" 'cat < a.txt > out.txt' 'cat out.txt'
ms_case "< and >> together" 'cat < b.txt >> a.txt' 'cat a.txt'
ms_case "> before <" 'cat > out.txt < a.txt' 'cat out.txt'
ms_case "the file name can be quoted" 'cat < "a b.txt"'

exit $pc_error
