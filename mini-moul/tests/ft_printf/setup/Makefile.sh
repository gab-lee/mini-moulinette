#!/bin/bash

# Checks the project's Makefile against the subject: it must exist, build
# libftprintf.a with the mandatory flags, not relink when nothing changed,
# and provide clean/fclean/re rules. The libftprintf.a built here is what
# every later ft_printf test links against. Run by test.sh with the
# mini-moul directory as cwd; the student's project is at ../

source ./config.sh

error=0
make_out=.make_out
marker=.make_marker

if [ ! -f ../Makefile ]; then
	printf "    ${RED}[1] Makefile is missing at the root of your project${DEFAULT}\n"
	exit 1
fi
printf "  ${GREEN}${CHECKMARK}${GREY} [1] Makefile exists${DEFAULT}\n"

if grep -q -- "-Wall" ../Makefile && grep -q -- "-Wextra" ../Makefile \
	&& grep -q -- "-Werror" ../Makefile; then
	printf "  ${GREEN}${CHECKMARK}${GREY} [2] Makefile uses -Wall -Wextra -Werror${DEFAULT}\n"
else
	printf "    ${RED}[2] Makefile must compile with -Wall -Wextra -Werror${DEFAULT}\n"
	error=1
fi

if make --no-print-directory -C .. > "$make_out" 2>&1; then
	printf "  ${GREEN}${CHECKMARK}${GREY} [3] make runs without error${DEFAULT}\n"
	if [ -f ../libftprintf.a ]; then
		printf "  ${GREEN}${CHECKMARK}${GREY} [4] make produced libftprintf.a${DEFAULT}\n"
		# 1s gap so a relink is visible on filesystems with 1s timestamps
		touch "$marker"
		sleep 1
		make -C .. > /dev/null 2>&1
		if [ ../libftprintf.a -nt "$marker" ]; then
			printf "    ${RED}[5] a second make relinked libftprintf.a although nothing changed${DEFAULT}\n"
			error=1
		else
			printf "  ${GREEN}${CHECKMARK}${GREY} [5] make does not relink when nothing changed${DEFAULT}\n"
		fi
		rm -f "$marker"
	else
		printf "    ${RED}[4] make succeeded but did not produce libftprintf.a${DEFAULT}\n"
		error=1
	fi
else
	printf "    ${RED}[3] make failed:${DEFAULT}\n"
	sed 's/^/    /' "$make_out" | head -15
	error=1
fi

i=6
for rule in clean fclean re; do
	if make -C .. -n "$rule" > /dev/null 2>&1; then
		printf "  ${GREEN}${CHECKMARK}${GREY} [$i] rule '$rule' exists${DEFAULT}\n"
	else
		printf "    ${RED}[$i] rule '$rule' is missing${DEFAULT}\n"
		error=1
	fi
	i=$((i+1))
done

rm -f "$make_out"
exit $error
