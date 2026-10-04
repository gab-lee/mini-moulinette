#ifndef TRY_H
#define TRY_H

/*
** Helpers for the manual drivers run by `mini --try <function> [arg ...]`.
**
** Each driver turns its command-line arguments into the function's C
** types, calls the student's function (and, where one exists, the libc
** reference on a separate copy of the input) and prints both results.
** Arguments:
**   string  decoded: \n \t \v \f \r \a \b \e \\ \0 and \xHH; @null is NULL
**   number  base 10; a negative size_t wraps (-1 is SIZE_MAX)
**   char    one character is taken as is; anything longer is its value
** Everything the driver allocates is freed at exit, so a LeakSanitizer
** report always points at the student's code.
**
** A driver returns 0 (done, or same result as libc), 1 (differs from
** libc) or 2 (bad arguments, usage printed).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include "../utils/constants.h"
#include "../tests/libft/libft_proto.h"

#define TRY_BOLD "\033[1m"

/* ---- allocations owned by the driver, freed at exit ---- */

static inline void	**try_pool(size_t *count)
{
	static void		*pool[256];
	static size_t	n;

	if (count)
		*count = n++;
	return (pool);
}

static inline void	try_cleanup(void)
{
	void	**pool;
	size_t	n;
	size_t	i;

	pool = try_pool(&n);
	i = 0;
	while (i < n && i < 256)
		free(pool[i++]);
}

static inline void	*try_alloc(size_t size)
{
	static int	registered;
	void		*p;
	size_t		slot;

	if (!registered)
	{
		atexit(try_cleanup);
		registered = 1;
	}
	p = calloc(size ? size : 1, 1);
	if (!p)
	{
		printf(RED "out of memory\n" DEFAULT);
		exit(2);
	}
	try_pool(&slot);
	if (slot >= 256)
	{
		printf(RED "too many arguments\n" DEFAULT);
		exit(2);
	}
	try_pool(NULL)[slot] = p;
	return (p);
}

/* ---- usage ---- */

static inline const char	*try_usage_text(const char *set)
{
	static const char	*usage;

	if (set)
		usage = set;
	return (usage);
}

static inline void	try_usage(const char *why)
{
	if (why)
		printf(RED "%s\n" DEFAULT, why);
	printf("usage: mini --try %s\n", try_usage_text(NULL));
	exit(2);
}

/* Exits with the usage line unless min <= argc - 1 <= max. */
static inline void	try_args(int argc, int min, int max, const char *usage)
{
	try_usage_text(usage);
	if (argc - 1 < min || argc - 1 > max)
		try_usage(argc - 1 < min ? "missing argument" : "too many arguments");
}

/* ---- argument parsing ---- */

static inline int	try_hex(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

/* Decodes escapes into out (at least strlen(arg) + 1 bytes); returns the
** decoded length, not counting the NUL added after it. */
static inline size_t	try_decode(const char *arg, char *out)
{
	size_t	n;
	char	c;

	n = 0;
	while (*arg)
	{
		if (*arg != '\\' || !arg[1])
		{
			out[n++] = *arg++;
			continue ;
		}
		c = arg[1];
		arg += 2;
		if (c == 'n')
			out[n++] = '\n';
		else if (c == 't')
			out[n++] = '\t';
		else if (c == 'v')
			out[n++] = '\v';
		else if (c == 'f')
			out[n++] = '\f';
		else if (c == 'r')
			out[n++] = '\r';
		else if (c == 'a')
			out[n++] = '\a';
		else if (c == 'b')
			out[n++] = '\b';
		else if (c == 'e')
			out[n++] = '\033';
		else if (c == '0')
			out[n++] = '\0';
		else if (c == '\\')
			out[n++] = '\\';
		else if (c == 'x' && try_hex(arg[0]) >= 0 && try_hex(arg[1]) >= 0)
		{
			out[n++] = (char)(try_hex(arg[0]) * 16 + try_hex(arg[1]));
			arg += 2;
		}
		else
		{
			out[n++] = '\\';
			out[n++] = c;
		}
	}
	out[n] = '\0';
	return (n);
}

static inline int	try_is_null(const char *arg)
{
	return (strcmp(arg, "@null") == 0);
}

/* A string argument, or NULL for @null, in a block of exactly the decoded
** length + 1 bytes. len (if given) gets the decoded length, which counts
** bytes after an embedded \0. */
static inline char	*try_str(const char *arg, size_t *len)
{
	char	*tmp;
	char	*s;
	size_t	n;

	if (len)
		*len = 0;
	if (try_is_null(arg))
		return (NULL);
	tmp = malloc(strlen(arg) + 1);
	if (!tmp)
		try_usage("out of memory");
	n = try_decode(arg, tmp);
	s = try_alloc(n + 1);
	memcpy(s, tmp, n + 1);
	free(tmp);
	if (len)
		*len = n;
	return (s);
}

/* A writable buffer holding the decoded argument, size bytes long (at
** least the decoded length + 1), zero-filled after the content, or NULL
** for @null. Sized exactly, so writing past it is a heap-buffer-overflow.
** total (if given) gets the buffer's size, 0 for NULL. */
static inline char	*try_buf(const char *arg, size_t size, size_t *total)
{
	char	*tmp;
	char	*buf;
	size_t	n;

	if (total)
		*total = 0;
	tmp = try_str(arg, &n);
	if (!tmp)
		return (NULL);
	if (size < n + 1)
		size = n + 1;
	buf = try_alloc(size);
	memcpy(buf, tmp, n);
	if (total)
		*total = size;
	return (buf);
}

static inline long	try_long(const char *arg, const char *name, long min, long max)
{
	char	*end;
	long	v;
	char	msg[128];

	errno = 0;
	v = strtol(arg, &end, 10);
	if (end == arg || *end || errno || v < min || v > max)
	{
		snprintf(msg, sizeof(msg), "%s must be a number from %ld to %ld",
			name, min, max);
		try_usage(msg);
	}
	return (v);
}

static inline size_t	try_size(const char *arg, const char *name)
{
	char				*end;
	unsigned long long	v;
	char				msg[128];

	if (arg[0] == '-')
		return ((size_t)try_long(arg, name, LONG_MIN, -1));
	errno = 0;
	v = strtoull(arg, &end, 10);
	if (end == arg || *end || errno || v > SIZE_MAX)
	{
		snprintf(msg, sizeof(msg), "%s must be a number", name);
		try_usage(msg);
	}
	return ((size_t)v);
}

/* One character is that character's byte value; anything longer is read
** as a number, so 'a' and 97 are the same, '7' is 55, and \0 is 0. */
static inline int	try_char(const char *arg, const char *name)
{
	char	*s;
	size_t	n;

	s = try_str(arg, &n);
	if (s && n == 1)
		return ((unsigned char)s[0]);
	return ((int)try_long(arg, name, INT_MIN, INT_MAX));
}

/* ---- printing ---- */

static inline void	try_put_byte(unsigned char c)
{
	if (c == '\n')
		printf("\\n");
	else if (c == '\t')
		printf("\\t");
	else if (c == '\v')
		printf("\\v");
	else if (c == '\f')
		printf("\\f");
	else if (c == '\r')
		printf("\\r");
	else if (c == '\0')
		printf("\\0");
	else if (c == '\\')
		printf("\\\\");
	else if (c == '"')
		printf("\\\"");
	else if (c < 32 || c >= 127)
		printf("\\x%02x", c);
	else
		printf("%c", c);
}

static inline void	try_put_str(const char *s)
{
	if (!s)
	{
		printf("NULL");
		return ;
	}
	printf("\"");
	while (*s)
		try_put_byte((unsigned char)*s++);
	printf("\"");
}

/* n bytes, NULs included */
static inline void	try_put_mem(const void *p, size_t n)
{
	const unsigned char	*b;
	size_t				i;

	if (!p)
	{
		printf("NULL");
		return ;
	}
	b = p;
	printf("\"");
	i = 0;
	while (i < n)
		try_put_byte(b[i++]);
	printf("\"");
}

static inline void	try_put_char(int c)
{
	if (c >= 0 && c <= 255)
	{
		printf("'");
		try_put_byte((unsigned char)c);
		printf("' (%d)", c);
	}
	else
		printf("%d", c);
}

/* A pointer into base, as its offset and the string it points at. */
static inline void	try_put_found(const char *base, const char *p)
{
	if (!p)
		printf("NULL");
	else
	{
		printf("offset %ld, ", (long)(p - base));
		try_put_str(p);
	}
}

/* Starts a result row: "  ft     ", "  libc   ", ... */
static inline void	try_row(const char *label)
{
	printf("  " GREY "%-7s" DEFAULT, label);
}

/* What a dst-returning function (memset, memcpy, ...) returned */
static inline void	try_put_ret(const void *ret, const void *dst)
{
	if (ret == dst)
		printf("returned dst");
	else if (!ret)
		printf("returned NULL");
	else
		printf("returned another pointer");
}

/* Prints why the libc reference is not called and returns 0 */
static inline int	try_no_ref(const char *ref, const char *why)
{
	try_row(ref);
	printf(GREY "not called: %s\n" DEFAULT, why);
	return (0);
}

/* The call line, e.g. ft_strlen("hello") */
static inline void	try_call(const char *name)
{
	printf(TRY_BOLD "%s" DEFAULT "(", name);
}

static inline void	try_call_sep(void)
{
	printf(", ");
}

static inline void	try_call_end(void)
{
	printf(")\n");
}

/* Prints the comparison verdict; returns the driver's exit status. */
static inline int	try_verdict(int same, const char *ref)
{
	if (same)
		printf("  " GREEN CHECKMARK " same result as %s\n" DEFAULT, ref);
	else
		printf("  " RED "x differs from %s\n" DEFAULT, ref);
	return (same ? 0 : 1);
}

/* Like try_verdict, for a check described by what was expected */
static inline int	try_expect(int ok, const char *expected)
{
	if (ok)
		printf("  " GREEN CHECKMARK " as expected: %s\n" DEFAULT, expected);
	else
		printf("  " RED "x expected %s\n" DEFAULT, expected);
	return (ok ? 0 : 1);
}

static inline int	try_sign(long v)
{
	return ((v > 0) - (v < 0));
}

/* ---- output of the ft_put*_fd functions ---- */

static inline FILE	**try_capture_file(void)
{
	static FILE	*f;

	return (&f);
}

/* The fd to hand the student's function: for fd 1 a temporary file, so the
** output can be shown escaped; any other fd is used as given. */
static inline int	try_capture_fd(int fd)
{
	FILE	*f;

	if (fd != 1)
		return (fd);
	f = tmpfile();
	if (!f)
		return (fd);
	*try_capture_file() = f;
	return (fileno(f));
}

/* Prints what was written; returns 1 if it is exactly expected (n bytes),
** or -1 when the output went to another fd and could not be checked. */
static inline int	try_capture_check(int fd, const char *expected, size_t n)
{
	FILE	*f;
	char	buf[4096];
	size_t	got;
	int		same;

	f = *try_capture_file();
	try_row("output");
	if (fd != 1 || !f)
	{
		printf(GREY "written to fd %d, not captured\n" DEFAULT, fd);
		return (-1);
	}
	fflush(f);
	rewind(f);
	got = fread(buf, 1, sizeof(buf), f);
	fclose(f);
	*try_capture_file() = NULL;
	try_put_mem(buf, got);
	printf(" (%zu byte%s)\n", got, got == 1 ? "" : "s");
	if (!expected)
		return (-1);
	try_row("expect");
	try_put_mem(expected, n);
	printf("\n");
	same = (got == n && memcmp(buf, expected, n) == 0);
	return (same);
}

/* ---- callbacks for ft_strmapi, ft_striteri, ft_lstiter, ft_lstmap ---- */

#define TRY_MAP_NAMES "toupper|tolower|addindex|digit"

static inline int	try_map_id(const char *name)
{
	if (strcmp(name, "toupper") == 0)
		return (0);
	if (strcmp(name, "tolower") == 0)
		return (1);
	if (strcmp(name, "addindex") == 0)
		return (2);
	if (strcmp(name, "digit") == 0)
		return (3);
	try_usage("unknown callback, use one of " TRY_MAP_NAMES);
	return (-1);
}

static inline int	*try_map_choice(void)
{
	static int	id;

	return (&id);
}

/* The chosen callback applied to character c at index i */
static inline char	try_map_apply(unsigned int i, char c)
{
	int	id;

	id = *try_map_choice();
	if (id == 0 && c >= 'a' && c <= 'z')
		return (c - 32);
	if (id == 1 && c >= 'A' && c <= 'Z')
		return (c + 32);
	if (id == 2)
		return ((char)(c + i));
	if (id == 3)
		return ((char)('0' + i % 10));
	return (c);
}

/* ---- linked lists ---- */

static inline int	*try_del_count(void)
{
	static int	n;

	return (&n);
}

/* del callback handed to the student's functions: frees and counts */
static inline void	try_del(void *content)
{
	(*try_del_count())++;
	free(content);
}

/* A list with one node per argument, built without the student's code.
** Contents are malloc'd strings (@null gives a NULL content), so the
** student's del can free them. */
static inline t_list	*try_list(int count, char **args)
{
	t_list	*head;
	t_list	**tail;
	t_list	*node;
	char	*tmp;

	head = NULL;
	tail = &head;
	while (count-- > 0)
	{
		node = malloc(sizeof(*node));
		tmp = try_str(*args++, NULL);
		node->content = tmp ? strdup(tmp) : NULL;
		node->next = NULL;
		*tail = node;
		tail = &node->next;
	}
	return (head);
}

static inline void	try_free_list(t_list *lst)
{
	t_list	*next;

	while (lst)
	{
		next = lst->next;
		free(lst->content);
		free(lst);
		lst = next;
	}
}

static inline void	try_put_list(t_list *lst)
{
	int	n;

	n = 0;
	printf("[");
	while (lst && n < 1000)
	{
		if (n++)
			printf(", ");
		try_put_str(lst->content);
		lst = lst->next;
	}
	if (lst)
		printf(", ... (more than 1000 nodes, loop?)");
	printf("] (%d node%s)\n", n, n == 1 ? "" : "s");
}

#endif
