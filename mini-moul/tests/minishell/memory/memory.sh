#!/bin/bash

# minishell built with AddressSanitizer runs a long session over every
# feature: no memory error (overflow, use after free, double free).
# Leaks of forked children are not looked at here; see leaks.sh.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_sanitized || exit 0
ASAN_OPTIONS="${ASAN_OPTIONS:+$ASAN_OPTIONS:}detect_leaks=0"
export ASAN_OPTIONS
ms_prepare
MS_RUN_BIN="$MS_SAN_BIN"
ms_run_only "echo, quotes and variables" 'echo hello "a  b" '"'"'$x'"'"' $MS_A "$MS_B" $NOT_SET' 'echo -n x' 'echo $?' 'echo "$"'
ms_run_only "builtins" 'pwd' 'cd dir' 'pwd' 'cd ..' 'export MS_N=1 MS_M=2' 'unset MS_N' 'env | grep ^MS_' 'export 1BAD=x' 'unset 1BAD'
ms_run_only "redirections" 'echo hi > out.txt' 'echo more >> out.txt' 'cat < out.txt' 'cat < nonexistent' 'echo x > dir' '> empty2'
ms_run_only "pipes" 'echo hi | cat | cat | wc -c' 'nonexistent | cat' 'cat a.txt | grep alpha > out.txt' 'ls | nonexistent'
ms_run_only "heredocs" 'cat << EOF' '$MS_A' 'EOF' 'cat << '"'"'EOF'"'"' | wc -l' 'x' 'EOF' 'cat << A << B' 'a' 'A' 'b' 'B'
ms_run_only "syntax errors" 'echo hi |' '| echo hi' 'echo "open' 'echo '"'"'open' 'cat <<' 'echo >' 'echo | | cat'
ms_run_only "commands" 'ls' '/bin/ls' './script.sh' 'nonexistent' './nonexistent' './a.txt' './dir' '""' 'unset PATH' 'ls' 'export PATH=/bin' 'ls'
ms_run_only "long lines" "echo $(printf 'x%.0s' {1..3000})" "echo $(printf 'a | cat %.0s' {1..40}) done"
ms_run_only "empty lines and spaces" '' '   ' 'echo ok'

exit $pc_error
