#!/bin/bash

# * in the current directory (the order of the names is not compared).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare_bonus
MS_POST=ms_post_words
ms_case "* lists the files of the directory" 'echo *'
ms_case "*.txt" 'echo *.txt'
ms_case "a*" 'echo a*'
ms_case "*t" 'echo *t'
ms_case "*a*" 'echo *a*'
ms_case "a*.txt" 'echo a*.txt'
ms_case "*.nomatch stays as written" 'echo *.nomatch'
ms_case "d* matches a directory" 'echo d*'
ms_case "hidden files are not matched" 'echo *hidden*'
ms_case "a star in double quotes is text" 'echo "*"'
ms_case "a star in single quotes is text" "echo '*.txt'"
ms_case "* as an argument of an external command" 'ls *.txt'
ms_case "* in the new directory after cd" 'cd dir' 'echo *'
ms_case "two patterns" 'echo a* b*'
ms_case "* together with other words" 'echo start *.txt end'
ms_case "*** is the same as *" 'echo ***'

exit $pc_error
