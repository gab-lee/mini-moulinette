#!/bin/bash

# ft_memmove: a missing NULL-check after malloc, and a leaked scratch
# buffer, change neither the copied bytes nor the return value - the
# ft_memmove.c content/return-value test can pass either bug outright.
# This checks both directly instead. Run by test.sh with mini-moul as
# cwd, student project at ../

source ./config.sh

error=0
obj=.memmove_probe.o
err=.memmove_probe.err

if ! cc -Wall -Werror -Wextra -c ../ft_memmove.c -o "$obj" 2> "$err"; then
	printf "    ${RED}[1] ../ft_memmove.c does not compile${DEFAULT}\n"
	sed 's/^/    /' "$err"
	rm -f "$obj" "$err"
	exit 1
fi
rm -f "$err"

# --- Case 1: malloc failing must not crash ---------------------------
probe1=.memmove_probe_null.c
obj1=.memmove_probe_null.o
bin1=.memmove_probe_null
cat > "$probe1" << 'EOF'
#include <stddef.h>

void	*ft_memmove(void *dest, const void *src, size_t n);

int	main(void)
{
	char	dst[8];
	char	src[8];

	/* Large enough that a real malloc cannot satisfy it, chosen so
	   n * sizeof(void *) does not wrap back down to something small. */
	ft_memmove(dst, src, (size_t)1 << 50);
	return (0);
}
EOF
if cc -Wall -Wextra -c "$probe1" -o "$obj1" 2> /dev/null \
	&& cc -o "$bin1" "$obj1" "$obj" 2> /dev/null; then
	if "./$bin1" > /dev/null 2>&1; then
		printf "  ${GREEN}${CHECKMARK}${GREY} [1] ft_memmove does not crash when malloc fails (n far beyond available memory)${DEFAULT}\n"
	else
		printf "    ${RED}[1] ft_memmove crashed when malloc failed - its result is never NULL-checked before use${DEFAULT}\n"
		error=1
	fi
else
	printf "    ${RED}[1] could not build the malloc-failure probe${DEFAULT}\n"
	error=1
fi
rm -f "$probe1" "$obj1" "$bin1"

# --- Case 2: no leak over repeated normal use -------------------------
if command -v leaks > /dev/null 2>&1; then
	probe2=.memmove_probe_leak.c
	obj2=.memmove_probe_leak.o
	bin2=.memmove_probe_leak
	cat > "$probe2" << 'EOF'
#include <stddef.h>

void	*ft_memmove(void *dest, const void *src, size_t n);

int	main(void)
{
	char	dst[64];
	char	src[64];
	int		i;

	i = 0;
	while (i < 2000)
	{
		ft_memmove(dst, src, 64);
		i++;
	}
	return (0);
}
EOF
	if cc -Wall -Wextra -c "$probe2" -o "$obj2" 2> /dev/null \
		&& cc -o "$bin2" "$obj2" "$obj" 2> /dev/null; then
		leak_line="$(leaks --atExit -- "./$bin2" 2>&1 \
			| grep -E '^Process [0-9]+: [0-9]+ leaks? for')"
		if printf '%s' "$leak_line" | grep -q '^Process [0-9]*: 0 leaks'; then
			printf "  ${GREEN}${CHECKMARK}${GREY} [2] ft_memmove does not leak its scratch buffer over 2000 calls${DEFAULT}\n"
		else
			printf "    ${RED}[2] ft_memmove leaks: %s${DEFAULT}\n" "$leak_line"
			error=1
		fi
	else
		printf "    ${RED}[2] could not build the leak probe${DEFAULT}\n"
		error=1
	fi
	rm -f "$probe2" "$obj2" "$bin2"
else
	printf "  ${GREEN}${CHECKMARK}${GREY} [!] [2] leak check skipped: 'leaks' is macOS-only and isn't available on this platform${DEFAULT}\n"
fi

rm -f "$obj"
exit $error
