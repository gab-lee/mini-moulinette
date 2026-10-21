#!/bin/bash

# minishell built with AddressSanitizer, leak detection on, runs commands
# that need no child process: everything allocated for a line, and
# everything held at exit, must have been freed. Leaks inside readline and
# ncurses are ignored.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_sanitized || exit 0
ms_lsan_suppressions
export LSAN_OPTIONS
ms_prepare
MS_RUN_BIN="$MS_SAN_BIN"
ms_run_only "echo and quotes" 'echo hello' 'echo "a  b" '"'"'c d'"'"'' 'echo -n x' 'echo $MS_A "$MS_B" $NOT_SET $?' 'echo "$"'
ms_run_only "cd and pwd" 'pwd' 'cd dir' 'pwd' 'cd ..' 'cd nonexistent' 'cd a.txt'
ms_run_only "export, unset and env" 'export MS_N=1 MS_M=2' 'export MS_N=3' 'export MS_NOVALUE' 'unset MS_N' 'unset NOT_SET' 'export 1BAD=x' 'unset 1BAD' 'env'
ms_run_only "redirections of builtins" 'echo hi > out.txt' 'echo more >> out.txt' 'pwd > out2.txt' 'echo x > dir' 'echo x > nonexistent_dir/y'
ms_run_only "syntax errors" 'echo hi |' '| echo hi' 'echo "open' 'echo '"'"'open' 'cat <<' 'echo >' 'echo > > x'
ms_run_only "empty lines" '' '   ' 'echo ok'
ms_run_only "exit" 'echo before' 'exit 3'
ms_run_only "end of input" 'echo before'

exit $pc_error
