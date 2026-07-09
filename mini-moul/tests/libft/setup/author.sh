#!/bin/bash

# Checks the author file at the project root: it must exist (named
# exactly 'author'), contain your login, and end with a newline —
# i.e. 'cat -e author' shows login$.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh

error=0

if [ ! -f ../author ]; then
	printf "    ${RED}[1] author file is missing at the root of your project${DEFAULT}\n"
	for wrong in AUTHOR Author authors auteur; do
		if [ -f "../$wrong" ]; then
			printf "    ${RED}    (found '%s' — the file must be named exactly 'author')${DEFAULT}\n" "$wrong"
			break
		fi
	done
	exit 1
fi
printf "  ${GREEN}${CHECKMARK}${GREY} [1] author file exists${DEFAULT}\n"

if [ -s ../author ]; then
	printf "  ${GREEN}${CHECKMARK}${GREY} [2] author file is not empty${DEFAULT}\n"
else
	printf "    ${RED}[2] author file is empty; it must contain your login${DEFAULT}\n"
	exit 1
fi

if [ "$(tail -c 1 ../author)" = "" ]; then
	printf "  ${GREEN}${CHECKMARK}${GREY} [3] author file ends with a newline${DEFAULT}\n"
else
	printf "    ${RED}[3] author file must end with a newline (cat -e should show 'login\$')${DEFAULT}\n"
	error=1
fi

exit $error
