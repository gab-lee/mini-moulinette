#!/bin/bash

# ctrl-C, ctrl-D and ctrl-\ in interactive mode, as bash does them. They
# need a terminal, so minishell runs in a pseudo-terminal driven by
# python3 (ms_pty.py). Without python3 the check is skipped with a [!].
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

if ! command -v python3 > /dev/null; then
	pc_warn "python3 was not found, so ctrl-C, ctrl-D and ctrl-\\ were not checked"
	exit 0
fi
pc_ensure_built .. minishell || exit 1
MS_BIN="$(cd .. && pwd)/minishell"
while IFS='|' read -r status label detail; do
	case "$status" in
		ok) pc_ok "$label" ;;
		bad) pc_bad "$label: $detail" ;;
	esac
done < <(python3 -I tests/minishell/ms_pty.py "$MS_BIN" 2> /dev/null)
if [ "$pc_i" -eq 1 ]; then
	pc_warn "the terminal checks did not run (no pseudo-terminal available?)"
fi
exit $pc_error
