#!/bin/bash

# ft_strcat/ft_strncat are NOT part of the libft subject - they're not
# declared in the subject's libft.h and won't be linked into the graded
# library, so this never scores against anyone who doesn't happen to
# have these files lying around: it's a silent no-op (exit 0) whenever
# ../ft_strcat.c / ../ft_strncat.c are absent, for both functions
# independently. If present, checks their most common bug: strcat's
# contract is to return the original dest pointer, and a common mistake
# (walking dest to its end with `while (*dest) dest++;` and never
# saving the original) returns a pointer into the middle of the result
# instead - content-only comparison can't see this, since dest's
# in-place content is still correct either way.
# Run by test.sh with mini-moul as cwd, student project at ../

source ./config.sh

error=0
found_any=0

check_one()
{
	local fn="$1"
	local decl="$2"
	local call="$3"
	local expected="$4"
	local probe=".${fn}_probe.c"
	local obj=".${fn}_probe.o"
	local bin=".${fn}_probe"
	local src_obj=".${fn}_src.o"

	[ -f "../$fn.c" ] || return 0
	found_any=1
	if ! cc -Wall -Werror -Wextra -c "../$fn.c" -o "$src_obj" 2> ".${fn}_src.err"; then
		printf "    ${RED}[%s] your %s.c does not compile${DEFAULT}\n" "$fn" "$fn"
		sed 's/^/    /' ".${fn}_src.err"
		rm -f "$src_obj" ".${fn}_src.err"
		error=1
		return 0
	fi
	rm -f ".${fn}_src.err"
	cat > "$probe" << EOF
#include <string.h>
#include <stdio.h>

$decl

int	main(void)
{
	char	dest[64];
	char	*ret;

	strcpy(dest, "Hello ");
	ret = $call;
	if (strcmp(dest, "$expected") != 0)
	{
		printf("content: expected \"$expected\", got \"%s\"\n", dest);
		return (1);
	}
	if (ret != dest)
	{
		printf("pointer: %s must return its dest argument unchanged "
			"(got dest+%ld instead)\n", "$fn", (long)(ret - dest));
		return (1);
	}
	return (0);
}
EOF
	if ! cc -Wall -Wextra -c "$probe" -o "$obj" 2> /dev/null \
		|| ! cc -o "$bin" "$obj" "$src_obj" 2> /dev/null; then
		printf "    ${RED}[!] could not build the $fn probe${DEFAULT}\n"
		rm -f "$probe" "$obj" "$bin" "$src_obj"
		error=1
		return 0
	fi
	probe_out="$("./$bin" 2>&1)"
	if [ $? -eq 0 ]; then
		printf "  ${GREEN}${CHECKMARK}${GREY} [%s] %s returns its dest pointer unchanged${DEFAULT}\n" "$fn" "$fn"
	else
		printf "    ${RED}[%s] %s: %s${DEFAULT}\n" "$fn" "$fn" "$probe_out"
		error=1
	fi
	rm -f "$probe" "$obj" "$bin" "$src_obj"
}

check_one "ft_strcat" "char	*ft_strcat(char *dest, char *src);" \
	'ft_strcat(dest, "World")' "Hello World"
check_one "ft_strncat" "char	*ft_strncat(char *dest, char *src, int n);" \
	'ft_strncat(dest, "World", 5)' "Hello World"

[ "$found_any" = "0" ] && exit 0
exit $error
