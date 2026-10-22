#!/bin/bash

# cd with a relative or an absolute path, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "cd to a subdirectory" 'cd dir' 'pwd'
ms_case "cd with a trailing slash" 'cd dir/' 'pwd'
ms_case "cd ./dir" 'cd ./dir' 'pwd'
ms_case "cd down and back up" 'cd dir' 'cd ..' 'pwd'
ms_case "cd ." 'cd .' 'pwd'
ms_case "cd .." 'cd ..' 'pwd'
ms_case "cd /" 'cd /' 'pwd'
ms_case "cd /usr" 'cd /usr' 'pwd'
ms_case "cd with an absolute path" "cd $MS_SANDBOX/dir" 'pwd'
ms_case "cd dir then ../dir" 'cd dir' 'cd ../dir' 'pwd'
ms_case "cd dir/../dir" 'cd dir/../dir' 'pwd'
ms_case "cd to a missing directory leaves the directory alone" 'cd nonexistent' 'pwd'
ms_case "cd to a missing directory sets the status to 1" 'cd nonexistent' 'echo $?'
ms_case "cd to a file sets the status to 1" 'cd a.txt' 'echo $?'
ms_case "the shell goes on after a failed cd" 'cd nonexistent' 'echo after'
ms_case "commands run in the new directory" 'cd dir' 'ls'
ms_case "commands read files of the new directory" 'cd dir' 'cat inner.txt'
ms_case "files created after cd land in the new directory" 'cd dir' 'echo hi > made.txt' 'cd ..' 'cat dir/made.txt'

exit $pc_error
