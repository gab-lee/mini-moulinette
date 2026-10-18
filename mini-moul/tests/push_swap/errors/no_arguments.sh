#!/bin/bash

# Without parameters the program displays nothing and gives the prompt back.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/push_swap/ps_harness.sh

ps_prepare push_swap

ps_run
problem="$(ps_crash_text)"
if [ -n "$problem" ]; then
	pc_bad "./push_swap with no argument: $problem"
elif [ -s .ps_out ] || [ -s .ps_err ]; then
	pc_bad "./push_swap with no argument must display nothing"
	pc_detail "stdout: $(head -c 60 .ps_out | head -1)"
	pc_detail "stderr: $(head -c 60 .ps_err | head -1)"
else
	pc_ok "no argument: nothing displayed"
fi
exit $pc_error
