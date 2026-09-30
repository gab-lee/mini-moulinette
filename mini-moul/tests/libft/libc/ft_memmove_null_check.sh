#!/bin/bash

# ft_memmove's leaked scratch buffer needs no dedicated check here: with
# CC_FLAGS now always including -fsanitize=address (see test.sh),
# LeakSanitizer is bundled into ASan automatically on Linux (unlike
# macOS/arm64, where it isn't available at all) and runs at process
# exit by default - the existing ft_memmove.c content/return-value test
# already calls ft_memmove several times, so on Linux, where grading
# actually happens, that plain test now also fails on the leak with no
# extra code. This file covers what that still can't: a missing
# NULL-check after malloc changes neither the copied bytes nor the
# return value, so content comparison alone can't see it either.
#
# Calls ft_memmove with an n far beyond any real allocation - large
# enough that malloc must fail, but chosen so n * sizeof(void *) does
# not integer-overflow back down to something small and accidentally
# succeed. A crash means the NULL result was never checked before use.
#
# Run by test.sh with mini-moul as cwd, student project at ../

source ./config.sh

obj=.memmove_probe.o
err=.memmove_probe.err
probe=.memmove_probe_null.c
probe_obj=.memmove_probe_null.o
bin=.memmove_probe_null

cleanup()
{
	rm -f "$obj" "$err" "$probe" "$probe_obj" "$bin"
}
trap cleanup EXIT

if ! cc -Wall -Werror -Wextra -c ../ft_memmove.c -o "$obj" 2> "$err"; then
	printf "    ${RED}../ft_memmove.c does not compile${DEFAULT}\n"
	sed 's/^/    /' "$err"
	exit 1
fi

cat > "$probe" << 'EOF'
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
if ! cc -Wall -Wextra -c "$probe" -o "$probe_obj" 2> /dev/null \
	|| ! cc -o "$bin" "$probe_obj" "$obj" 2> /dev/null; then
	printf "    ${RED}could not build the malloc-failure probe${DEFAULT}\n"
	exit 1
fi
if "./$bin" > /dev/null 2>&1; then
	printf "  ${GREEN}${CHECKMARK}${GREY} ft_memmove does not crash when malloc fails (n far beyond available memory)${DEFAULT}\n"
	exit 0
fi
printf "    ${RED}ft_memmove crashed when malloc failed - its result is never NULL-checked before use${DEFAULT}\n"
exit 1
