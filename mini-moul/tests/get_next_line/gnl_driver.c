#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "get_next_line_proto.h"
#include "../../utils/constants.h"

/*
** get_next_line test driver. gnl_harness.sh links it against the student's
** files built with one BUFFER_SIZE and runs it as
**
**   gnl_<size> <scenario> <label> <buffer size, 0 if unknown> <instr|plain>
**
** Every case runs in a forked child killed after GNL_TIMEOUT seconds, so a
** crash, an infinite loop or a read() that never returns is reported
** against that one case. A case's messages go through a pipe to the parent,
** which prints them under the case's header only if it failed.
**
** In "instr" mode the student's files were compiled with
** -Dmalloc=gnl_test_malloc -Dfree=gnl_test_free -Dread=gnl_test_read, so a
** case can count blocks still allocated after get_next_line returns NULL,
** check that returned lines are real malloc'd blocks, see how far
** get_next_line read ahead, and make read() fail.
*/

#define GNL_TIMEOUT 5
#define GNL_MAGIC 0x676e6c21UL
#define MAX_TRACKED_FD 1024
#define DETAIL_MAX 16384
#define MAX_SHOWN 3

typedef struct s_case
{
	const char	*desc;
	int			(*fn)(int arg);
	int			arg;
	const char	*timeout_hint;
}	t_case;

typedef struct s_stream
{
	int			fd;
	const char	*name;
	const char	*content;
	size_t		len;
	size_t		pos;
	int			calls;
}	t_stream;

void	*gnl_test_malloc(size_t size);
void	gnl_test_free(void *ptr);
ssize_t	gnl_test_read(int fd, void *buf, size_t nbytes);

static int	g_instr;
static long	g_bs;
static long	g_live_blocks;
static long	g_live_bytes;
static int	g_bad_free;
static int	g_fail_read;
static int	g_warned;
static long	g_bytes_read[MAX_TRACKED_FD];

/* ---------------------------------------------------------------------- */
/* Instrumentation (only reached in "instr" mode)                          */
/* ---------------------------------------------------------------------- */

void	*gnl_test_malloc(size_t size)
{
	size_t	*block;

	block = malloc(size + 2 * sizeof(size_t));
	if (block == NULL)
		return (NULL);
	block[0] = size;
	block[1] = GNL_MAGIC;
	g_live_blocks++;
	g_live_bytes += size;
	return (block + 2);
}

void	gnl_test_free(void *ptr)
{
	size_t	*block;

	if (ptr == NULL)
		return ;
	block = (size_t *)ptr - 2;
	if (block[1] != GNL_MAGIC)
	{
		g_bad_free++;
		return ;
	}
	block[1] = 0;
	g_live_blocks--;
	g_live_bytes -= block[0];
	free(block);
}

ssize_t	gnl_test_read(int fd, void *buf, size_t nbytes)
{
	ssize_t	r;

	if (g_fail_read)
	{
		errno = EIO;
		return (-1);
	}
	r = read(fd, buf, nbytes);
	if (r > 0 && fd >= 0 && fd < MAX_TRACKED_FD)
		g_bytes_read[fd] += r;
	return (r);
}

/* ---------------------------------------------------------------------- */
/* Helpers used inside a case (child process)                              */
/* ---------------------------------------------------------------------- */

static void	free_line(char *line)
{
	if (g_instr)
		gnl_test_free(line);
	else
		free(line);
}

/* Prints up to 40 bytes of s, escaped, then its length.
** Avoids putchar() on purpose: constants.h #defines it. */
static void	put_escaped(const char *s, size_t len)
{
	size_t	i;
	size_t	shown;

	shown = len > 40 ? 40 : len;
	printf("\"");
	i = 0;
	while (i < shown)
	{
		if (s[i] == '\n')
			printf("\\n");
		else if (s[i] == '\t')
			printf("\\t");
		else if (s[i] == '"' || s[i] == '\\')
			printf("\\%c", s[i]);
		else if ((unsigned char)s[i] < 32 || (unsigned char)s[i] == 127)
			printf("\\x%02x", (unsigned char)s[i]);
		else
			printf("%c", s[i]);
		i++;
	}
	printf("%s\" (%zu bytes)", shown < len ? "..." : "", len);
}

static void	put_call(const t_stream *s)
{
	printf(RED "        call %d", s->calls);
	if (s->name)
		printf(" on %s", s->name);
	printf(": ");
}

/* Writes content to a temp file and returns a read-only fd to it. The file
** is unlinked straight away, so nothing is left behind even on a crash. */
static int	open_content(const char *content, size_t len)
{
	char	path[64];
	int		fd;

	snprintf(path, sizeof(path), ".gnl_case_%d.txt", (int)getpid());
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0 || write(fd, content, len) != (ssize_t)len)
	{
		perror(path);
		exit(2);
	}
	close(fd);
	fd = open(path, O_RDONLY);
	unlink(path);
	return (fd);
}

static void	init_stream(t_stream *s, int fd, const char *name,
	const char *content)
{
	s->fd = fd;
	s->name = name;
	s->content = content;
	s->len = strlen(content);
	s->pos = 0;
	s->calls = 0;
}

/* After a line ending at byte `end` was returned: a correct get_next_line
** has read at most end - 1 + BUFFER_SIZE bytes, since it stops calling
** read() once it has a '\n'. Reading the whole input on the first call
** (slurping the file) is a failure; any other over-read, such as one
** extra read() per call, is only a [!] warning. */
static int	check_overread(const t_stream *s, size_t end)
{
	long	got;

	if (!g_instr || g_bs <= 0 || s->fd < 0 || s->fd >= MAX_TRACKED_FD)
		return (0);
	got = g_bytes_read[s->fd];
	if (got < (long)end + g_bs)
		return (0);
	if (s->calls == 1 && got >= (long)s->len
		&& (long)s->len >= (long)end + g_bs)
	{
		put_call(s);
		printf("get_next_line read the whole input (%ld bytes) on its first "
			"call, to return a line that ends at byte %zu; return the line as "
			"soon as you find its '\\n'\n" DEFAULT, got, end);
		return (1);
	}
	if (!g_warned)
	{
		g_warned = 1;
		printf(YELLOW "  [!] call %d%s%s: read %ld bytes to return a line "
			"ending at byte %zu; with BUFFER_SIZE=%ld it never needs more "
			"than %ld (read as little as possible)\n" DEFAULT,
			s->calls, s->name ? " on " : "", s->name ? s->name : "",
			got, end, g_bs, (long)end - 1 + g_bs);
	}
	return (0);
}

/* One get_next_line call on s, checked against the next expected line, or
** against NULL once the content is used up. Returns 0 on a match. */
static int	step(t_stream *s)
{
	char		*line;
	const char	*nl;
	size_t		end;

	s->calls++;
	line = get_next_line(s->fd);
	if (s->pos >= s->len)
	{
		if (line == NULL)
			return (0);
		put_call(s);
		printf("expected NULL (nothing left to read), got ");
		put_escaped(line, strlen(line));
		printf("\n" DEFAULT);
		free_line(line);
		return (1);
	}
	nl = memchr(s->content + s->pos, '\n', s->len - s->pos);
	end = nl ? (size_t)(nl - s->content) + 1 : s->len;
	if (line == NULL || strlen(line) != end - s->pos
		|| memcmp(line, s->content + s->pos, end - s->pos) != 0)
	{
		put_call(s);
		printf("expected ");
		put_escaped(s->content + s->pos, end - s->pos);
		printf(", got ");
		if (line)
			put_escaped(line, strlen(line));
		else
			printf("NULL");
		printf("\n" DEFAULT);
		free_line(line);
		return (1);
	}
	free_line(line);
	if (g_bad_free)
	{
		put_call(s);
		printf("the returned line is not the start of a malloc'd block, "
			"so it cannot be freed\n" DEFAULT);
		return (1);
	}
	if (check_overread(s, end))
		return (1);
	s->pos = end;
	return (0);
}

/* Reads s to the end, then checks that two more calls return NULL. */
static int	read_all(t_stream *s)
{
	while (s->pos < s->len)
		if (step(s))
			return (1);
	return (step(s) || step(s));
}

static int	expect_null(int fd, const char *what)
{
	char	*line;

	line = get_next_line(fd);
	if (line == NULL)
		return (0);
	printf(RED "        get_next_line(%s) should return NULL, got ", what);
	put_escaped(line, strlen(line));
	printf("\n" DEFAULT);
	free_line(line);
	return (1);
}

static int	check_leaks(void)
{
	if (!g_instr || g_live_blocks == 0)
		return (0);
	printf(RED "        memory leak: %ld block(s), %ld bytes still allocated "
		"after get_next_line returned NULL\n" DEFAULT,
		g_live_blocks, g_live_bytes);
	return (1);
}

/* ---------------------------------------------------------------------- */
/* Content for the regular-file cases                                      */
/* ---------------------------------------------------------------------- */

static char	*long_line(size_t n, const char *tail)
{
	char	*s;
	size_t	i;

	s = malloc(n + strlen(tail) + 1);
	i = 0;
	while (i < n)
	{
		s[i] = 'a' + i % 26;
		i++;
	}
	strcpy(s + n, tail);
	return (s);
}

static char	*numbered_lines(int count)
{
	char	*s;
	int		i;
	size_t	len;

	s = malloc(count * 32 + 1);
	len = 0;
	i = 1;
	while (i <= count)
	{
		len += sprintf(s + len, "line %03d: some text\n", i);
		i++;
	}
	return (s);
}

static char	*growing_lines(int max)
{
	char	*s;
	size_t	len;
	int		i;
	int		j;

	s = malloc((size_t)max * (max + 1) + 1);
	len = 0;
	i = 1;
	while (i <= max)
	{
		j = 0;
		while (j < i)
			s[len++] = '0' + j++ % 10;
		s[len++] = '\n';
		i++;
	}
	s[len] = '\0';
	return (s);
}

static const char	*file_content(int which)
{
	static const char	*fixed[] = {
		"Hello, World!\n",
		"Hello, World!",
		"first\nsecond\nthird\n",
		"first\nsecond\nthird",
		"",
		"\n",
		"\n\n\n\n",
		"a\n\nb\n\n\nc\n",
		"x",
	};

	if (which < 9)
		return (fixed[which]);
	if (which == 9)
		return (long_line(10000, "\nafter the long line\n"));
	if (which == 10)
		return (long_line(10000, ""));
	if (which == 11)
		return (numbered_lines(100));
	return (growing_lines(60));
}

/* ---------------------------------------------------------------------- */
/* Cases                                                                   */
/* ---------------------------------------------------------------------- */

static int	case_file(int which)
{
	t_stream	s;
	const char	*content;

	content = file_content(which);
	init_stream(&s, open_content(content, strlen(content)), NULL, content);
	return (read_all(&s) || check_leaks());
}

static int	case_stdin_file(int arg)
{
	t_stream	s;
	const char	*content;
	int			fd;

	(void)arg;
	content = "one\ntwo\nthree";
	fd = open_content(content, strlen(content));
	dup2(fd, STDIN_FILENO);
	close(fd);
	init_stream(&s, STDIN_FILENO, "fd 0", content);
	return (read_all(&s) || check_leaks());
}

static int	case_stdin_pipe(int arg)
{
	t_stream	s;
	int			p[2];
	char		*content;

	(void)arg;
	content = numbered_lines(50);
	strcat(content, "last line, no newline");
	if (pipe(p) != 0)
		return (perror("pipe"), 1);
	if (write(p[1], content, strlen(content)) != (ssize_t)strlen(content))
		return (perror("write"), 1);
	close(p[1]);
	dup2(p[0], STDIN_FILENO);
	close(p[0]);
	init_stream(&s, STDIN_FILENO, "fd 0", content);
	return (read_all(&s) || check_leaks());
}

/* The write end stays open, so a read() past the first line blocks until
** the timeout kills the case. */
static int	case_pipe_open(int arg)
{
	t_stream	s;
	int			p[2];
	const char	*second;

	(void)arg;
	second = "second line\n";
	if (pipe(p) != 0)
		return (perror("pipe"), 1);
	if (write(p[1], "first line\n", 11) != 11)
		return (perror("write"), 1);
	init_stream(&s, p[0], "a pipe", "first line\nsecond line\n");
	if (step(&s))
		return (1);
	if (write(p[1], second, strlen(second)) != (ssize_t)strlen(second))
		return (perror("write"), 1);
	close(p[1]);
	return (read_all(&s) || check_leaks());
}

static int	case_bad_fd(int which)
{
	int			fd;
	const char	*what;
	int			error;

	if (which == 0)
	{
		fd = -1;
		what = "-1";
	}
	else if (which == 1)
	{
		fd = 1000;
		close(fd);
		what = "1000, a fd that was never opened";
	}
	else if (which == 2)
	{
		fd = open_content("some text\n", 10);
		close(fd);
		what = "a closed fd";
	}
	else
	{
		fd = open(".", O_RDONLY);
		what = "a directory";
	}
	error = expect_null(fd, what) || expect_null(fd, what);
	return (error || check_leaks());
}

static int	case_read_error(int arg)
{
	t_stream	s;
	int			fd;

	(void)arg;
	if (!g_instr)
		return (0);
	g_fail_read = 1;
	fd = open_content("abc\ndef\n", 8);
	if (expect_null(fd, "fd whose read() fails"))
		return (1);
	close(fd);
	if (check_leaks())
		return (1);
	g_fail_read = 0;
	init_stream(&s, open_content("one\ntwo\n", 8), "a new file", "one\ntwo\n");
	return (read_all(&s) || check_leaks());
}

static int	case_recover(int arg)
{
	t_stream	s;

	(void)arg;
	close(1000);
	if (expect_null(-1, "-1") || expect_null(1000, "1000"))
		return (1);
	init_stream(&s, open_content("one\ntwo\n", 8), "a file", "one\ntwo\n");
	return (read_all(&s) || check_leaks());
}

/* Bonus: calls step() on every stream in turn, `rounds` times. */
static int	round_robin(t_stream *s, int count, int rounds)
{
	int	r;
	int	i;

	r = 0;
	while (r < rounds)
	{
		i = 0;
		while (i < count)
			if (step(&s[i++]))
				return (1);
		r++;
	}
	return (0);
}

static int	case_multi(int which)
{
	t_stream	s[3];
	const char	*c[3];
	int			p[2];

	if (which == 0)
	{
		c[0] = "a1\na2\na3\n";
		c[1] = "b1\nb2\nb3\n";
		c[2] = "c1\nc2\nc3\n";
	}
	else
	{
		c[0] = "the only line\n";
		c[1] = "b1\nb2\nb3\nb4\n";
		c[2] = "c1\nc2";
	}
	if (which <= 1)
	{
		init_stream(&s[0], open_content(c[0], strlen(c[0])), "file a", c[0]);
		init_stream(&s[1], open_content(c[1], strlen(c[1])), "file b", c[1]);
		init_stream(&s[2], open_content(c[2], strlen(c[2])), "file c", c[2]);
		return (round_robin(s, 3, 6) || check_leaks());
	}
	if (which == 2)
	{
		init_stream(&s[0], open_content(c[1], strlen(c[1])), "file a", c[1]);
		init_stream(&s[1], open_content(c[2], strlen(c[2])), "file b", c[2]);
		close(1000);
		if (step(&s[0]) || expect_null(-1, "-1") || step(&s[1])
			|| expect_null(1000, "1000"))
			return (1);
		return (round_robin(s, 2, 5) || check_leaks());
	}
	if (which == 3)
	{
		c[0] = "pipe 1\npipe 2\npipe 3";
		if (pipe(p) != 0 || write(p[1], c[0], strlen(c[0])) < 0)
			return (perror("pipe"), 1);
		close(p[1]);
		init_stream(&s[0], p[0], "a pipe", c[0]);
		init_stream(&s[1], open_content(c[1], strlen(c[1])), "a file", c[1]);
		return (round_robin(s, 2, 6) || check_leaks());
	}
	c[0] = long_line(3000, "\nend of a\n");
	init_stream(&s[0], open_content(c[0], strlen(c[0])), "file a", c[0]);
	init_stream(&s[1], open_content(c[1], strlen(c[1])), "file b", c[1]);
	return (round_robin(s, 2, 6) || check_leaks());
}

/* ---------------------------------------------------------------------- */
/* Scenarios                                                               */
/* ---------------------------------------------------------------------- */

#define PIPE_HINT "get_next_line kept calling read() after it already had \
a full line, and a pipe waits for input that has not been written yet; \
return the line as soon as you find its '\\n'"

static const t_case	g_file_cases[] = {
	{"one line ending with '\\n'", case_file, 0, NULL},
	{"one line, no '\\n' at the end", case_file, 1, NULL},
	{"three lines", case_file, 2, NULL},
	{"three lines, the last one without '\\n'", case_file, 3, NULL},
	{"empty file", case_file, 4, NULL},
	{"a file that is just '\\n'", case_file, 5, NULL},
	{"a file of only '\\n's", case_file, 6, NULL},
	{"empty lines between text", case_file, 7, NULL},
	{"one character, no '\\n'", case_file, 8, NULL},
	{"a 10000-character line, then a short one", case_file, 9, NULL},
	{"a 10000-character line with no '\\n'", case_file, 10, NULL},
	{"100 lines", case_file, 11, NULL},
	{"lines of every length from 1 to 60", case_file, 12, NULL},
};

static const t_case	g_stdin_cases[] = {
	{"standard input redirected from a file", case_stdin_file, 0, NULL},
	{"standard input from a pipe", case_stdin_pipe, 0, NULL},
	{"returns a pipe's first line without waiting for more input",
		case_pipe_open, 0, PIPE_HINT},
};

static const t_case	g_error_cases[] = {
	{"get_next_line(-1)", case_bad_fd, 0, NULL},
	{"a fd that was never opened", case_bad_fd, 1, NULL},
	{"a closed fd", case_bad_fd, 2, NULL},
	{"a directory (read() fails)", case_bad_fd, 3, NULL},
	{"read() returns -1: NULL, nothing leaked, next file reads fine",
		case_read_error, 0, NULL},
	{"a normal file still reads correctly after invalid fds", case_recover,
		0, NULL},
};

static const t_case	g_multi_cases[] = {
	{"three files read in turn, one line each", case_multi, 0, NULL},
	{"three files of different lengths read in turn", case_multi, 1, NULL},
	{"invalid fds between two files do not disturb them", case_multi, 2,
		NULL},
	{"a pipe and a file read in turn", case_multi, 3, NULL},
	{"a 3000-character line in one file, short lines in another",
		case_multi, 4, NULL},
};

/* Runs c in a child; its output lands in details. Returns 0 pass, 1 fail,
** 2 crash (signal in *sig), 3 timeout. */
static int	run_case(const t_case *c, char *details, int *sig)
{
	int		fds[2];
	pid_t	pid;
	int		status;
	ssize_t	n;
	size_t	total;
	char	sink[512];

	fflush(stdout);
	if (pipe(fds) != 0)
	{
		perror("pipe");
		exit(2);
	}
	pid = fork();
	if (pid == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		alarm(GNL_TIMEOUT);
		status = c->fn(c->arg);
		fflush(stdout);
		_exit(status != 0);
	}
	close(fds[1]);
	total = 0;
	while (total < DETAIL_MAX - 1
		&& (n = read(fds[0], details + total, DETAIL_MAX - 1 - total)) > 0)
		total += n;
	details[total] = '\0';
	while (read(fds[0], sink, sizeof(sink)) > 0)
		;
	close(fds[0]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		*sig = WTERMSIG(status);
		return (*sig == SIGALRM ? 3 : 2);
	}
	return (WEXITSTATUS(status) != 0);
}

static void	report(const t_case *c, int i, const char *label, int result,
	int sig, const char *details)
{
	printf(RED "    [%d] %s: %s\n" DEFAULT, i, label, c->desc);
	printf("%s", details);
	if (result == 2)
	{
		printf(RED "        crashed: %s", strsignal(sig));
		if ((sig == SIGSEGV || sig == SIGBUS) && g_bs >= 1000000)
			printf(" (an array of BUFFER_SIZE bytes on the stack is too big "
				"for it; allocate the buffer with malloc)");
		printf("\n" DEFAULT);
	}
	else if (result == 3)
		printf(RED "        timed out after %ds: %s\n" DEFAULT, GNL_TIMEOUT,
			c->timeout_hint ? c->timeout_hint
			: "infinite loop, or a read() waiting for input that never comes");
}

int	main(int argc, char **argv)
{
	const t_case	*cases;
	int				count;
	int				i;
	int				failed;
	int				result;
	int				sig;
	int				warned;
	static char		details[DETAIL_MAX];

	if (argc != 5)
	{
		fprintf(stderr, "usage: %s <file|stdin|errors|multi> <label> "
			"<buffer size> <instr|plain>\n", argv[0]);
		return (2);
	}
	g_bs = atol(argv[3]);
	g_instr = strcmp(argv[4], "instr") == 0;
	if (strcmp(argv[1], "file") == 0)
	{
		cases = g_file_cases;
		count = sizeof(g_file_cases) / sizeof(*g_file_cases);
	}
	else if (strcmp(argv[1], "stdin") == 0)
	{
		cases = g_stdin_cases;
		count = sizeof(g_stdin_cases) / sizeof(*g_stdin_cases);
	}
	else if (strcmp(argv[1], "errors") == 0)
	{
		cases = g_error_cases;
		count = sizeof(g_error_cases) / sizeof(*g_error_cases);
	}
	else
	{
		cases = g_multi_cases;
		count = sizeof(g_multi_cases) / sizeof(*g_multi_cases);
	}
	failed = 0;
	warned = 0;
	i = 0;
	while (i < count)
	{
		sig = 0;
		result = run_case(&cases[i], details, &sig);
		if (result == 0 && details[0] != '\0' && !warned)
		{
			printf("%s", details);
			warned = 1;
		}
		else
		{
			if (failed < MAX_SHOWN)
				report(&cases[i], i + 1, argv[2], result, sig, details);
			failed++;
		}
		if (result == 3 && i + 1 < count)
		{
			printf(RED "        (skipped the other %d case(s) for %s after "
				"the timeout)\n" DEFAULT, count - i - 1, argv[2]);
			break ;
		}
		i++;
	}
	if (failed > MAX_SHOWN)
		printf(RED "    ...and %d more failing case(s) for %s\n" DEFAULT,
			failed - MAX_SHOWN, argv[2]);
	if (failed == 0)
		printf("  " GREEN CHECKMARK GREY " %s: all %d cases passed\n" DEFAULT,
			argv[2], count);
	return (failed != 0);
}
