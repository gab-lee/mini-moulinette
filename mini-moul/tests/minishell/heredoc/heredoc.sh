#!/bin/bash

# << reads lines until the delimiter, compared with bash.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare
ms_case "one line" 'cat << EOF' 'hello' 'EOF'
ms_case "several lines" 'cat << EOF' 'one' 'two' 'three' 'EOF'
ms_case "an empty heredoc" 'cat << EOF' 'EOF'
ms_case "an empty line inside" 'cat << EOF' 'a' '' 'b' 'EOF'
ms_case "<< without a space before the delimiter" 'cat <<EOF' 'hello' 'EOF'
ms_case "another delimiter" 'cat << STOP' 'hello' 'STOP'
ms_case "a line that only contains the delimiter ends it" 'cat << EOF' 'a' 'EOF' 'echo b'
ms_case "a line that holds the delimiter but is not it does not end it" 'cat << EOF' 'EOF2' ' EOF' 'EOF '"'"'x' 'EOF'
ms_case "the variable \$MS_A is expanded" 'cat << EOF' '$MS_A and $MS_B' 'EOF'
ms_case "\$? is expanded" 'false' 'cat << EOF' '$?' 'EOF'
ms_case "a quoted delimiter stops the expansion" 'cat << '"'"'EOF'"'"'' '$MS_A' 'EOF'
ms_case "a double quoted delimiter stops the expansion" 'cat << "EOF"' '$MS_A' 'EOF'
ms_case "quotes in the text are kept" 'cat << EOF' '"quoted" and '"'"'single'"'"'' 'EOF'
ms_case "a pipe sign in the text is text" 'cat << EOF' 'a | b > c' 'EOF'
ms_case "the heredoc output goes to a file" 'cat << EOF > out.txt' 'hello' 'EOF' 'cat out.txt'
ms_case "the heredoc feeds a pipe" 'cat << EOF | wc -l' 'a' 'b' 'c' 'EOF'
ms_case "the heredoc then another command" 'cat << EOF' 'hello' 'EOF' 'echo after'
ms_case "two heredocs in a row" 'cat << A' 'one' 'A' 'cat << B' 'two' 'B'
ms_case "with two << only the last one is read by the command" 'cat << A << B' 'x' 'A' 'y' 'B'
ms_case "the heredoc replaces an earlier <" 'cat < a.txt << EOF' 'heredoc' 'EOF'
ms_case "a later < replaces the heredoc" 'cat << EOF < a.txt' 'heredoc' 'EOF'
ms_case "the heredoc with grep" 'grep b << EOF' 'abc' 'xyz' 'EOF'
ms_case "the heredoc with wc" 'wc -l << EOF' 'a' 'b' 'EOF'
ms_case "the heredoc status is the command status" 'grep zzz << EOF' 'abc' 'EOF' 'echo $?'

exit $pc_error
