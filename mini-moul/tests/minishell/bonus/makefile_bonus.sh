#!/bin/bash

# The Makefile has a bonus rule that builds minishell, and a second make
# bonus does not relink.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

if make -C .. -n bonus > /dev/null 2>&1; then
	pc_ok "rule 'bonus' exists"
else
	pc_bad "rule 'bonus' is missing"
	exit 1
fi
if make -C .. bonus > .make_out 2>&1; then
	pc_ok "make bonus runs without error"
else
	pc_bad "make bonus failed:"
	sed 's/^/    /' .make_out | head -15
	rm -f .make_out
	exit 1
fi
rm -f .make_out
if [ -x ../minishell ]; then
	pc_ok "make bonus produced minishell"
else
	pc_bad "make bonus did not produce minishell"
	exit 1
fi
if make -C .. -n bonus 2> /dev/null | grep -Eq '(^|[[:space:]/])(ar|cc|gcc|clang)[[:space:]]'; then
	pc_bad "make bonus relinks: a second make bonus with nothing changed still runs:"
	make -C .. -n bonus 2> /dev/null | grep -E '(^|[[:space:]/])(ar|cc|gcc|clang)[[:space:]]' \
		| sed 's/^/    /' | head -5
else
	pc_ok "make bonus does not relink when nothing changed"
fi
exit $pc_error
