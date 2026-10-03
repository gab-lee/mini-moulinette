#!/bin/bash

# check_prototypes "<header>" "name|pointer-decl|subject prototype" ...
#
# Verifies that the student's <header> (relative to the project root,
# e.g. "libft.h" or "ft_printf.h") declares each function with the
# exact type the subject defines. For every entry a tiny probe is
# compiled that assigns the function to a pointer of the subject's
# type; -Werror turns any deviation (return type, parameter types,
# missing const, ...) into a compile error. C never checks types at
# link time, so this is the only place a wrong signature gets caught.
#
# Sourced by each part's prototypes.sh; cwd is the mini-moul directory,
# the student's project is at ../

check_prototypes()
{
	local header="$1"
	shift
	local error=0
	local i=1
	local entry name decl pretty theirs
	local probe=.proto_probe.c
	local obj=.proto_probe.o
	local err=.proto_probe.err

	if [ ! -f "../$header" ]; then
		printf "    ${RED}%s is missing at the root of your project; cannot check prototypes${DEFAULT}\n" "$header"
		return 1
	fi
	for entry in "$@"; do
		IFS='|' read -r name decl pretty <<< "$entry"
		{
			printf '#include "../%s"\n\n' "$header"
			printf 'void probe(void)\n{\n'
			printf '\t%s = %s;\n\t(void)p;\n}\n' "$decl" "$name"
		} > "$probe"
		if cc -Wall -Werror -Wextra -c "$probe" -o "$obj" 2> "$err"; then
			printf "  ${GREEN}${CHECKMARK}${GREY} [$i] %s is prototyped as the subject defines${DEFAULT}\n" "$name"
		elif grep -q "undeclared" "$err"; then
			printf "    ${RED}[$i] %s is not declared in %s (expected: %s;)${DEFAULT}\n" \
				"$name" "$header" "$pretty"
			error=1
		else
			printf "    ${RED}[$i] %s: prototype differs from the subject${DEFAULT}\n" "$name"
			printf "    ${RED}    subject: %s;${DEFAULT}\n" "$pretty"
			theirs="$(grep -E "[^a-zA-Z0-9_]$name[[:space:]]*\(" "../$header" | head -1 | sed 's/^[[:space:]]*//')"
			[ -n "$theirs" ] && printf "    ${RED}    yours:   %s${DEFAULT}\n" "$theirs"
			error=1
		fi
		i=$((i+1))
	done
	rm -f "$probe" "$obj" "$err"
	return $error
}
