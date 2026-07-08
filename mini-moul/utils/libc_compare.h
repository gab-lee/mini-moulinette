#ifndef LIBC_COMPARE_H
#define LIBC_COMPARE_H

/*
** Helpers for testing the libc part of libft by comparing the ft_ function
** against the real libc function, instead of hardcoding expected values.
**
** - check_truthy: for the is* family. libc only promises zero/nonzero, so
**   only agreement on true/false is compared, never the exact number.
** - check_exact: for toupper/tolower and anything returning a real value.
** - sweep_truthy / sweep_exact: run the comparison for every value from
**   EOF (-1) through 255, catching cases no named test thought of.
**
** Every failure message prints both sides (ft_ result and libc result) so
** the bug is visible from the terminal without opening the test file.
*/

#include <stdio.h>
#include "constants.h"

/* Render a character value for messages: 'a' for printables, code otherwise */
static inline void print_char_value(int c)
{
	if (c >= 32 && c <= 126)
		printf("'%c' (%d)", c, c);
	else
		printf("%d", c);
}

/* Compare yes/no behaviour. Returns 0 on agreement, -1 on mismatch. */
static inline int check_truthy(int i, char *desc, int mine, int ref)
{
	if ((mine != 0) == (ref != 0))
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s matches libc (%s)\n" DEFAULT,
			i, desc, ref ? "true" : "false");
		return (0);
	}
	printf("    " RED "[%d] %s: libc says %s, ft version returned %d\n" DEFAULT,
		i, desc, ref ? "true (nonzero)" : "false (0)", mine);
	return (-1);
}

/* Compare exact return values. Returns 0 on match, -1 on mismatch. */
static inline int check_exact(int i, char *desc, int mine, int ref)
{
	if (mine == ref)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s returned " DEFAULT, i, desc);
		print_char_value(mine);
		printf(GREY " as expected\n" DEFAULT);
		return (0);
	}
	printf("    " RED "[%d] %s: libc returned " DEFAULT RED, i, desc);
	print_char_value(ref);
	printf(", ft version returned ");
	print_char_value(mine);
	printf("\n" DEFAULT);
	return (-1);
}

/* Sweep EOF..255 comparing truthiness. Prints up to 5 mismatches. */
static inline int sweep_truthy(int i, char *name, int (*mine)(int), int (*ref)(int))
{
	int c;
	int mismatches = 0;

	for (c = -1; c <= 255; c++)
	{
		if ((mine(c) != 0) != (ref(c) != 0))
		{
			if (++mismatches <= 5)
			{
				printf("    " RED "[%d] %s sweep mismatch at c=" DEFAULT RED, i, name);
				print_char_value(c);
				printf(": libc says %s, ft version returned %d\n" DEFAULT,
					ref(c) ? "true" : "false", mine(c));
			}
		}
	}
	if (mismatches == 0)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s matches libc for every value EOF..255\n" DEFAULT,
			i, name);
		return (0);
	}
	if (mismatches > 5)
		printf("    " RED "[%d] %s sweep: %d mismatches in total\n" DEFAULT, i, name, mismatches);
	return (-1);
}

/* Sweep EOF..255 comparing exact values. Prints up to 5 mismatches. */
static inline int sweep_exact(int i, char *name, int (*mine)(int), int (*ref)(int))
{
	int c;
	int mismatches = 0;

	for (c = -1; c <= 255; c++)
	{
		if (mine(c) != ref(c))
		{
			if (++mismatches <= 5)
			{
				printf("    " RED "[%d] %s sweep mismatch at c=" DEFAULT RED, i, name);
				print_char_value(c);
				printf(": libc returned ");
				print_char_value(ref(c));
				printf(", ft version returned ");
				print_char_value(mine(c));
				printf("\n" DEFAULT);
			}
		}
	}
	if (mismatches == 0)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s matches libc for every value EOF..255\n" DEFAULT,
			i, name);
		return (0);
	}
	if (mismatches > 5)
		printf("    " RED "[%d] %s sweep: %d mismatches in total\n" DEFAULT, i, name, mismatches);
	return (-1);
}

#endif
