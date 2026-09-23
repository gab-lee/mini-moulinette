#ifndef PRINTF_COMPARE_H
# define PRINTF_COMPARE_H

/*
** Helpers for testing ft_printf by comparing its output and return value
** against the real printf, instead of hand-computing expected strings.
**
** Each call runs in a forked child with fd 1 redirected to a temp file, so
** a crash or an infinite loop in one case is reported against that case
** (the child is killed after PF_TIMEOUT seconds) instead of taking down
** the whole test file. Captured output is compared by exact byte range
** (length + memcmp), not strcmp, so an embedded NUL byte in the output
** (e.g. "%c" with argument 0) can't silently truncate the comparison.
**
** Usage in a test file:
**
**   int ret_mine, ret_ref;
**   char *out_mine, *out_ref;
**   long len_mine, len_ref;
**
**   PF_RUN(ft_printf, ret_mine, out_mine, len_mine, "%c", 'A');
**   PF_RUN(real_printf, ret_ref, out_ref, len_ref, "%c", 'A');
**   error += check_printf(1, "ft_printf(\"%c\", 'A')",
**       ret_mine, out_mine, len_mine, ret_ref, out_ref, len_ref);
**   free(out_mine);
**   free(out_ref);
**
** If the call crashed, out_var is NULL and len_var holds the signal number
** (0 if the child exited without returning); check_printf reports it.
**
** Reference calls must go through real_printf, not printf() directly:
** some of the most useful cases here are deliberately unusual (conflicting
** flags, zero-length formats, ...) and GCC's static format-string checker
** (-Wformat, part of -Wall) rejects several of those as compile errors on
** a direct printf() call even though the real printf handles them fine at
** runtime. Calling through a plain function pointer has no format
** attribute attached, so the static check doesn't run.
*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "constants.h"

# define PF_TIMEOUT 3

static int (* const real_printf)(const char *, ...) = printf;

# define PF_RUN(fn, ret_var, out_var, len_var, ...) \
	do { \
		char pf_tmp_path[64]; \
		int pf_pipe[2]; \
		pid_t pf_pid; \
		int pf_ret; \
		pf_prepare(pf_tmp_path, pf_pipe); \
		pf_pid = fork(); \
		if (pf_pid == 0) \
		{ \
			pf_child_start(pf_tmp_path, pf_pipe); \
			pf_ret = fn(__VA_ARGS__); \
			pf_child_end(pf_pipe, pf_ret); \
		} \
		(out_var) = pf_collect(pf_pid, pf_tmp_path, pf_pipe, \
			&(ret_var), &(len_var)); \
	} while (0)

static inline void pf_prepare(char *tmp_path, int *fds)
{
	fflush(stdout);
	snprintf(tmp_path, 64, ".printf_capture_%d.tmp", (int)getpid());
	if (pipe(fds) != 0)
	{
		perror("pipe");
		exit(1);
	}
}

/* In the child: send fd 1 to the capture file and arm the timeout. */
static inline void pf_child_start(char *tmp_path, int *fds)
{
	int fd;

	close(fds[0]);
	fd = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	alarm(PF_TIMEOUT);
}

static inline void pf_child_end(int *fds, int ret)
{
	fflush(stdout);
	if (write(fds[1], &ret, sizeof(ret)) != sizeof(ret))
		_exit(1);
	_exit(0);
}

/* In the parent: waits for the child and returns the captured bytes as a
** malloc'd buffer (NUL-terminated for convenience, but *len is the real
** byte count and is what comparisons must use), or NULL if the child
** crashed, timed out or never returned. Caller must free() it. */
static inline char *pf_collect(pid_t pid, char *tmp_path, int *fds,
	int *ret, long *len)
{
	int status;
	long got;
	int fd;
	long size;
	char *buf;

	close(fds[1]);
	waitpid(pid, &status, 0);
	got = read(fds[0], ret, sizeof(*ret));
	close(fds[0]);
	fd = open(tmp_path, O_RDONLY);
	size = (fd < 0) ? 0 : lseek(fd, 0, SEEK_END);
	buf = malloc(size + 1);
	if (fd >= 0)
	{
		lseek(fd, 0, SEEK_SET);
		if (size > 0 && read(fd, buf, size) != size)
			size = 0;
		close(fd);
	}
	buf[size] = '\0';
	unlink(tmp_path);
	*len = size;
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0
		|| got != (long)sizeof(*ret))
	{
		free(buf);
		*len = WIFSIGNALED(status) ? WTERMSIG(status) : 0;
		return (NULL);
	}
	return (buf);
}

/* Prints 'len' bytes with control characters escaped, for failure messages.
** Avoids putchar() on purpose: constants.h #defines it to custom_putchar. */
static inline void pf_print_str(const char *s, long len)
{
	long k;

	k = 0;
	while (k < len)
	{
		if (s[k] == '\n')
			printf("\\n");
		else if (s[k] == '\t')
			printf("\\t");
		else if (s[k] == '"')
			printf("\\\"");
		else if (s[k] == '\\')
			printf("\\\\");
		else if ((unsigned char)s[k] < 32 || (unsigned char)s[k] == 127)
			printf("\\x%02x", (unsigned char)s[k]);
		else
			printf("%c", s[k]);
		k++;
	}
}

/* Compares captured output (by exact byte range) and return value.
** Returns 0 on match, -1 on mismatch (both are checked and reported
** independently). */
static inline int check_printf(int i, char *desc,
	int mine_ret, char *mine_out, long mine_len,
	int ref_ret, char *ref_out, long ref_len)
{
	int output_matches;

	if (mine_out == NULL)
	{
		printf("    " RED "[%d] %s\n" DEFAULT, i, desc);
		if (mine_len == SIGALRM)
			printf("    " RED "    timed out after %ds (infinite loop?)\n" DEFAULT,
				PF_TIMEOUT);
		else if (mine_len > 0)
			printf("    " RED "    crashed: %s\n" DEFAULT, strsignal((int)mine_len));
		else
			printf("    " RED "    exited before returning\n" DEFAULT);
		return (-1);
	}
	output_matches = (mine_len == ref_len)
		&& (ref_len == 0 || memcmp(mine_out, ref_out, ref_len) == 0);
	if (mine_ret == ref_ret && output_matches)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
		return (0);
	}
	printf("    " RED "[%d] %s\n" DEFAULT, i, desc);
	if (mine_ret != ref_ret)
		printf("    " RED "    return value: expected %d, got %d\n" DEFAULT,
			ref_ret, mine_ret);
	if (!output_matches)
	{
		printf("    " RED "    output: expected \"");
		pf_print_str(ref_out, ref_len);
		printf("\" (%ld bytes)\n" DEFAULT, ref_len);
		printf("    " RED "    output: got      \"");
		pf_print_str(mine_out, mine_len);
		printf("\" (%ld bytes)\n" DEFAULT, mine_len);
	}
	return (-1);
}

#endif
