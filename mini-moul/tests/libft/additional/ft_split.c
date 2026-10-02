#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libft_proto.h"
#include "../../../utils/constants.h"
#include "../../../utils/alloc_check.h"

static void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static void	print_escaped(char const *s)
{
	if (!s)
	{
		printf("(null)");
		return ;
	}
	printf("\"");
	while (*s)
	{
		if (*s == '\t')
			printf("\\t");
		else if (*s == '\n')
			printf("\\n");
		else if (*s == '\v')
			printf("\\v");
		else if (*s == '\f')
			printf("\\f");
		else if (*s == '\r')
			printf("\\r");
		else
			printf("%c", *s);
		s++;
	}
	printf("\"");
}

/* The array must hold exactly expected_count + 1 pointers and each word
** exactly strlen + 1 bytes. Reports the first mismatch only. */
static int	split_sizes_ok(int i, char *desc, char **res, char **expected,
			int expected_count)
{
	int		k;
	size_t	got;
	size_t	want;

	got = ALLOC_USABLE_SIZE(res);
	want = alloc_expected_size(sizeof(char *) * (expected_count + 1));
	if (got != want)
	{
		printf("    " RED "[%d] %s: array allocated %zu bytes, expected %zu (%d pointer(s) incl. the NULL)\n" DEFAULT,
			i, desc, got, want, expected_count + 1);
		return (-1);
	}
	k = 0;
	while (k < expected_count)
	{
		got = ALLOC_USABLE_SIZE(res[k]);
		want = alloc_expected_size(strlen(expected[k]) + 1);
		if (got != want)
		{
			printf("    " RED "[%d] %s: element %d allocated %zu bytes, expected %zu\n" DEFAULT,
				i, desc, k, got, want);
			return (-1);
		}
		k++;
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s allocates the exact array and word sizes\n" DEFAULT,
		i, desc);
	return (0);
}

static int	split_case(int i, char *desc, char const *s, char c,
			char **expected, int expected_count)
{
	char	**res;
	int		k;

	res = ft_split(s, c);
	if (res == NULL)
	{
		printf("    " RED "[%d] %s: returned NULL\n" DEFAULT, i, desc);
		return (-1);
	}
	k = 0;
	while (k < expected_count)
	{
		if (res[k] == NULL || strcmp(res[k], expected[k]) != 0)
		{
			printf("    " RED "[%d] %s: element %d expected ", i, desc, k);
			print_escaped(expected[k]);
			printf(", got ");
			print_escaped(res[k]);
			printf("\n" DEFAULT);
			free_split(res);
			return (-1);
		}
		k++;
	}
	if (res[expected_count] != NULL)
	{
		if (expected_count == 0)
			printf("    " RED "[%d] %s: expected an empty array, got a non-NULL first element\n" DEFAULT,
				i, desc);
		else
			printf("    " RED "[%d] %s: array has extra elements past index %d, or is not NULL-terminated\n" DEFAULT,
				i, desc, expected_count - 1);
		free_split(res);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
	k = split_sizes_ok(i, desc, res, expected, expected_count);
	free_split(res);
	return (k);
}

int main(void)
{
	int		error = 0;
	char	*words[] = {"hello", "world"};
	char	**none = NULL;
	char	*single[] = {"hello"};
	char	*three[] = {"a", "b", "c"};
	char	*ab[] = {"a", "b"};
	char	*hwf[] = {"hello", "world", "foo"};
	char	*sentence[] = {"The", "quick", "brown", "fox", "jumps", "over",
		"the", "lazy", "dog"};
	char	*whole[] = {"hello world"};
	char	*long_words[] = {"abcdefghijklmnopqrstuvwxyz", "0123456789",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	char	*tab_kept[] = {"a\tb", "c"};
	char	*ws_kept[] = {"one\ttwo\nthree\vfour\ffive\rsix"};
	char	*ws_split[] = {"a b", "c d"};
	char	*four[] = {"alpha", "beta", "gamma", "delta"};
	char	*many[] = {"w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7", "w8",
		"w9", "w10", "w11", "w12", "w13", "w14", "w15", "w16", "w17", "w18",
		"w19"};
	char	*trip42[] = {"tripouille", "42"};
	char	*trip[] = {"tripouille"};
	char	*chini[] = {"chinimala"};
	char	*tripcap[] = {"Tripouille"};
	char	*dashes[] = {"1", "2", "3", "4", "5", "42"};

	error += split_case(1, "ft_split(\"hello world\", ' ') splits on a single space",
		"hello world", ' ', words, 2);
	error += split_case(2, "ft_split(\"  hello   world  \", ' ') collapses repeated separators",
		"  hello   world  ", ' ', words, 2);
	error += split_case(3, "ft_split(\"\", ' ') on empty string returns an empty array",
		"", ' ', none, 0);
	error += split_case(4, "ft_split(\"   \", ' ') on separators-only returns an empty array",
		"   ", ' ', none, 0);
	error += split_case(5, "ft_split(\"hello\", ' ') with no separator returns one element",
		"hello", ' ', single, 1);
	error += split_case(6, "ft_split(\"a,b,c\", ',') splits on comma",
		"a,b,c", ',', three, 3);
	error += split_case(7, "ft_split(\",a,,b,\", ',') ignores leading/trailing/doubled separators",
		",a,,b,", ',', ab, 2);
	error += split_case(8, "ft_split(\"hello world foo\", ' ') returns three filled words",
		"hello world foo", ' ', hwf, 3);
	error += split_case(9, "ft_split on a nine-word sentence keeps every word's content",
		"The quick brown fox jumps over the lazy dog", ' ', sentence, 9);
	error += split_case(10, "ft_split(\"hello world\", '\\0') returns the whole string as one element",
		"hello world", '\0', whole, 1);
	error += split_case(11, "ft_split with long words of differing lengths",
		"--abcdefghijklmnopqrstuvwxyz-0123456789---ABCDEFGHIJKLMNOPQRSTUVWXYZ-", '-',
		long_words, 3);
	error += split_case(12, "ft_split(\"\\thello\\t\\tworld\\tfoo\\t\", '\\t') splits on tab",
		"\thello\t\tworld\tfoo\t", '\t', hwf, 3);
	error += split_case(13, "ft_split(\"hello\\nworld\\nfoo\", '\\n') splits on newline",
		"hello\nworld\nfoo", '\n', hwf, 3);
	error += split_case(14, "ft_split(\"\\vhello\\vworld\\v\\vfoo\", '\\v') splits on vertical tab",
		"\vhello\vworld\v\vfoo", '\v', hwf, 3);
	error += split_case(15, "ft_split(\"hello\\fworld\\ffoo\\f\", '\\f') splits on form feed",
		"hello\fworld\ffoo\f", '\f', hwf, 3);
	error += split_case(16, "ft_split(\"hello\\rworld\\rfoo\", '\\r') splits on carriage return",
		"hello\rworld\rfoo", '\r', hwf, 3);
	error += split_case(17, "ft_split(\"a\\tb c\", ' ') keeps the tab inside a word",
		"a\tb c", ' ', tab_kept, 2);
	error += split_case(18, "ft_split(\"  one\\ttwo\\n...six  \", ' ') treats other whitespace as ordinary characters",
		"  one\ttwo\nthree\vfour\ffive\rsix  ", ' ', ws_kept, 1);
	error += split_case(19, "ft_split(\"\\ta b\\t\\tc d\\t\", '\\t') keeps spaces inside words",
		"\ta b\t\tc d\t", '\t', ws_split, 2);
	error += split_case(20, "ft_split(\"\\t\\t\\t\", '\\t') on tabs-only returns an empty array",
		"\t\t\t", '\t', none, 0);
	error += split_case(21, "ft_split(\"alpha beta gamma delta\", ' ') returns four words",
		"alpha beta gamma delta", ' ', four, 4);
	error += split_case(22, "ft_split on a twenty-word string returns every word",
		"w0 w1 w2 w3 w4 w5 w6 w7 w8 w9 w10 w11 w12 w13 w14 w15 w16 w17 w18 w19",
		' ', many, 20);
	error += split_case(23, "ft_split(\"  tripouille  42  \", ' ')",
		"  tripouille  42  ", ' ', trip42, 2);
	error += split_case(24, "ft_split(\"tripouille\", '\\0') returns one element",
		"tripouille", '\0', trip, 1);
	error += split_case(25, "ft_split(\"\", '\\0') returns an empty array",
		"", '\0', none, 0);
	error += split_case(26, "ft_split(\"chinimala\", ' ') returns one element",
		"chinimala", ' ', chini, 1);
	error += split_case(27, "ft_split(\"Tripouille \", ' ') trailing separator",
		"Tripouille ", ' ', tripcap, 1);
	error += split_case(28, "ft_split(\" Tripouille\", ' ') leading separator",
		" Tripouille", ' ', tripcap, 1);
	error += split_case(29, "ft_split(\" Tripouille \", ' ') separators on both sides",
		" Tripouille ", ' ', tripcap, 1);
	error += split_case(30, "ft_split(\"--1-2--3---4----5-----42\", '-')",
		"--1-2--3---4----5-----42", '-', dashes, 6);

	return (error);
}
