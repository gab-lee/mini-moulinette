#!/bin/bash

# Running commands by name, relative path and absolute path, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "a command found in PATH" 'ls'
ms_case "a command with arguments" 'ls -a dir'
ms_case "a command with an absolute path" '/bin/ls'
ms_case "an absolute path with arguments" '/bin/echo hi there'
ms_case "a script with a relative path" './script.sh'
ms_case "a script with a path going up and down" 'dir/../script.sh'
ms_case "a script is not in PATH" 'script.sh'
ms_case "several files as arguments" 'cat a.txt b.txt'
ms_case "a command that does not exist" 'nonexistent_command'
ms_case "a path that does not exist" './nonexistent'
ms_case "a file without execute permission" './a.txt'
ms_case "a directory as a command" './dir'
ms_case "an empty command in quotes" '""'
ms_case "a PATH made of one directory" 'export PATH=/bin' 'ls'
ms_case "a command out of reach once PATH changes" 'export PATH=/nonexistent' 'ls'
ms_case "an absolute path still works when PATH is wrong" 'export PATH=/nonexistent' '/bin/echo hi'
ms_case "unset PATH" 'unset PATH' 'ls'
ms_case "an absolute path works without PATH" 'unset PATH' '/bin/echo hi'
ms_case "a directory added to PATH" "export PATH=$MS_SANDBOX/dir:\$PATH" 'mytool'
ms_case "PATH with a directory that does not exist first" "export PATH=/nonexistent:$MS_PATH" 'ls'
ms_case "a command run twice" 'echo hi' 'echo hi'
ms_case "many commands one after the other" 'ls' 'pwd' 'echo a' 'cat a.txt' 'ls dir' 'echo b' 'true' 'false' 'echo $?'

exit $pc_error
