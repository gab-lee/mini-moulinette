#!/bin/bash

# Checks that libft.h exists and is usable, before any function is tested.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh

error=0
probe=.libft_h_probe.c
probe_bin=.libft_h_probe
probe_err=.libft_h_probe.err

if [ ! -f ../libft.h ]; then
	printf "    ${RED}[1] libft.h is missing at the root of your project${DEFAULT}\n"
	exit 1
fi
printf "  ${GREEN}${CHECKMARK}${GREY} [1] libft.h exists${DEFAULT}\n"

printf '#include "../libft.h"\n\nint main(void)\n{\n\treturn (0);\n}\n' > "$probe"
if cc -Wall -Werror -Wextra -o "$probe_bin" "$probe" 2> "$probe_err"; then
	printf "  ${GREEN}${CHECKMARK}${GREY} [2] libft.h compiles on its own${DEFAULT}\n"
else
	printf "    ${RED}[2] libft.h does not compile on its own:${DEFAULT}\n"
	sed 's/^/    /' "$probe_err" | head -15
	error=1
fi

printf '#include "../libft.h"\n#include "../libft.h"\n\nint main(void)\n{\n\treturn (0);\n}\n' > "$probe"
if cc -Wall -Werror -Wextra -o "$probe_bin" "$probe" 2> "$probe_err"; then
	printf "  ${GREEN}${CHECKMARK}${GREY} [3] libft.h survives double inclusion (include guard)${DEFAULT}\n"
else
	printf "    ${RED}[3] libft.h breaks when included twice; check your include guard:${DEFAULT}\n"
	sed 's/^/    /' "$probe_err" | head -15
	error=1
fi

rm -f "$probe" "$probe_bin" "$probe_err"
exit $error
