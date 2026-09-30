#ifndef ALLOC_CHECK_H
#define ALLOC_CHECK_H

/*
** Allocation-size check, ported from Tripouille/libftTester's mcheck().
**
** check_alloc_size compares the usable size of a block the student
** allocated against a fresh malloc(required), so an over- or
** under-sized allocation (e.g. ft_substr allocating len + 1 when the
** remaining string is shorter) is caught. Under AddressSanitizer (the
** default) usable sizes are exact; without it the allocator rounds both
** sides up the same way, so only a larger mismatch shows.
*/

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

#if defined(__APPLE__)
# include <malloc/malloc.h>
# define ALLOC_USABLE_SIZE(p) malloc_size(p)
#else
# include <malloc.h>
# define ALLOC_USABLE_SIZE(p) malloc_usable_size(p)
#endif

/* Usable size malloc(required) would give, for comparison. */
static inline size_t alloc_expected_size(size_t required)
{
	void	*ref;
	size_t	size;

	ref = malloc(required);
	size = ALLOC_USABLE_SIZE(ref);
	free(ref);
	return (size);
}

/* Returns 0 when p's block matches malloc(required), -1 otherwise. */
static inline int check_alloc_size(int i, char *desc, void *p, size_t required)
{
	size_t	got;
	size_t	want;

	if (p == NULL)
	{
		printf("    " RED "[%d] %s: returned NULL, expected a %zu-byte allocation\n" DEFAULT,
			i, desc, required);
		return (-1);
	}
	got = ALLOC_USABLE_SIZE(p);
	want = alloc_expected_size(required);
	if (got == want)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s allocates exactly %zu bytes\n" DEFAULT,
			i, desc, required);
		return (0);
	}
	printf("    " RED "[%d] %s: allocated %zu bytes, expected %zu (the exact size needed)\n" DEFAULT,
		i, desc, got, want);
	return (-1);
}

#endif
