#!/bin/bash

# > and >>, compared with bash (stdout and the files left behind).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "> creates the file" 'echo hi > out.txt' 'cat out.txt'
ms_case "> without spaces" 'echo hi>out.txt' 'cat out.txt'
ms_case "> truncates an existing file" 'echo first > out.txt' 'echo second > out.txt' 'cat out.txt'
ms_case "> overwrites a longer file" 'cat b.txt > out.txt' 'echo x > out.txt' 'cat out.txt'
ms_case ">> creates the file" 'echo hi >> out.txt' 'cat out.txt'
ms_case ">> appends" 'echo a >> out.txt' 'echo b >> out.txt' 'cat out.txt'
ms_case ">> appends to an existing file" 'echo more >> a.txt' 'cat a.txt'
ms_case ">> without spaces" 'echo hi>>out.txt' 'cat out.txt'
ms_case "the output of a command is not printed" 'echo hidden > out.txt'
ms_case "two > make both files, the last one gets the output" 'echo hi > out1.txt > out2.txt' 'cat out1.txt' 'cat out2.txt'
ms_case "> before the command" '> out.txt echo hi' 'cat out.txt'
ms_case "> between the arguments" 'echo a > out.txt b' 'cat out.txt'
ms_case "> without a command creates an empty file" '> out.txt' 'cat out.txt'
ms_case "> for an external command" 'ls > out.txt' 'cat out.txt'
ms_case "> to /dev/null" 'echo hi > /dev/null'
ms_case "> then >> on the same command" 'echo a > out.txt >> out.txt' 'cat out.txt'
ms_case "the file name can be quoted" 'echo hi > "out.txt"' 'cat out.txt'
ms_case "> with a builtin" 'pwd > out.txt' 'cat out.txt'
ms_case "> in the new directory after cd" 'cd dir' 'echo hi > out.txt' 'ls'

exit $pc_error
