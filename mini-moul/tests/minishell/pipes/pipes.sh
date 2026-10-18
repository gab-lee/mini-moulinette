#!/bin/bash

# | between commands, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "echo into cat" 'echo hi | cat'
ms_case "three commands" 'echo hi | cat | cat'
ms_case "nine commands" 'echo a | cat | cat | cat | cat | cat | cat | cat | cat'
ms_case "ls into wc" 'ls | wc -l'
ms_case "cat into grep" 'cat a.txt | grep alpha'
ms_case "cat into sort" 'cat b.txt | sort -r'
ms_case "cat into wc" 'cat a.txt | wc -c'
ms_case "tr in a pipe" 'echo a b c | tr '"'"' '"'"' -'
ms_case "a pipe without spaces" 'echo hi|cat'
ms_case "the last command writes into a file" 'echo hi | cat > out.txt' 'cat out.txt'
ms_case "the first command reads a file" 'cat < a.txt | cat > out.txt' 'cat out.txt'
ms_case "a pipe with a heredoc" 'cat << EOF | cat' 'hi' 'EOF'
ms_case "a producer that never stops, cut short by head" 'yes | head -n 3'
ms_case "a lot of data through a pipe" 'yes | head -n 100000 | wc -l'
ms_case "a pipe with an empty producer" 'cat < empty | cat'
ms_case "a pipe followed by another command" 'echo a | cat' 'echo b'
ms_case "the second command ignores the first one's output" 'echo x | echo y'
ms_case "pipes keep the quotes of each command" 'echo "a | b" | cat'
ms_case "a pipe and an expansion" 'echo $MS_A | cat'
ms_case "a pipe and the PATH lookup" 'echo hi | /bin/cat'

exit $pc_error
