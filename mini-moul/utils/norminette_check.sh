#!/bin/bash

# check_norminette
#
# Runs norminette against the student's project and reports it as a
# single PASS/FAIL check. Deliberately never prints norminette's own
# output (even on failure) - the raw error dump is what made the old
# standalone step distracting; run `norminette` yourself in the
# project for details. norminette is an optional local tool (not every
# student has it installed), so a missing binary is a skip, not a fail.
#
# Sourced by each part's norminette.sh; cwd is the mini-moul directory,
# the student's project is at ../

check_norminette()
{
	if ! command -v norminette &> /dev/null; then
		printf "    ${GREY}norminette not found, skipping norm check${DEFAULT}\n"
		return 0
	fi

	if norminette .. > /dev/null 2>&1; then
		printf "  ${GREEN}${CHECKMARK}${GREY} [1] norminette passes${DEFAULT}\n"
		return 0
	fi

	printf "    ${RED}[1] norminette found errors - run 'norminette' in your project for details${DEFAULT}\n"
	return 1
}
