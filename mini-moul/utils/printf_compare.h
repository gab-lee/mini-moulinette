#ifndef PRINTF_COMPARE_H
# define PRINTF_COMPARE_H

/*
** Helpers for testing ft_printf by comparing its output and return value
** against the real printf, instead of hand-computing expected strings.
**
** ft_printf writes straight to fd 1 (no FILE* buffering), so output is
** captured by redirecting fd 1 to a temp file around the call and reading
** it back — this works whether the student's implementation goes through
** write() or a buffered FILE* on their end.
**
** Usage in a test file:
**
**   int ret_mine, ret_ref;
**   char *out_mine, *out_ref;
**
**   PF_RUN(ft_printf, ret_mine, out_mine, "%c", 'A');
**   PF_RUN(printf, ret_ref, out_ref, "%c", 'A');
**   error += check_printf(1, "ft_printf(\"%c\", 'A')",
**       ret_mine, out_mine, ret_ref, out_ref);
**   free(out_mine);
**   free(out_ref);
*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include "constants.h"

# define PF_RUN(fn, ret_var, out_var, ...) \
	do { \
		char pf_tmp_path[64]; \
		int pf_saved_fd = pf_capture_start(pf_tmp_path); \
		(ret_var) = fn(__VA_ARGS__); \
		(out_var) = pf_capture_end(pf_saved_fd, pf_tmp_path); \
	} while (0)

/* Redirects fd 1 to a fresh temp file; returns a saved copy of the old fd 1. */
static inline int pf_capture_start(char *tmp_path)
{
	int fd;
	int saved;

	fflush(stdout);
	snprintf(tmp_path, 64, ".printf_capture_%d.tmp", (int)getpid());
	fd = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	saved = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (saved);
}

/* Restores fd 1 and returns the captured bytes as a malloc'd, NUL-terminated
** string. Caller must free() it. */
static inline char *pf_capture_end(int saved, char *tmp_path)
{
	int fd;
	long size;
	char *buf;

	fflush(stdout);
	dup2(saved, STDOUT_FILENO);
	close(saved);
	fd = open(tmp_path, O_RDONLY);
	size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	buf = malloc(size + 1);
	if (size > 0 && read(fd, buf, size) != size)
		size = 0;
	buf[size] = '\0';
	close(fd);
	unlink(tmp_path);
	return (buf);
}

/* Prints a string with control characters escaped, for failure messages.
** Avoids putchar() on purpose: constants.h #defines it to custom_putchar. */
static inline void pf_print_str(const char *s)
{
	while (*s)
	{
		if (*s == '\n')
			printf("\\n");
		else if (*s == '\t')
			printf("\\t");
		else if (*s == '"')
			printf("\\\"");
		else if (*s == '\\')
			printf("\\\\");
		else if ((unsigned char)*s < 32 || (unsigned char)*s == 127)
			printf("\\x%02x", (unsigned char)*s);
		else
			printf("%c", *s);
		s++;
	}
}

/* Compares captured output and return value. Returns 0 on match, -1 on
** mismatch (both are checked and reported independently). */
static inline int check_printf(int i, char *desc, int mine_ret, char *mine_out,
	int ref_ret, char *ref_out)
{
	if (mine_ret == ref_ret && strcmp(mine_out, ref_out) == 0)
	{
		printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT, i, desc);
		return (0);
	}
	printf("    " RED "[%d] %s\n" DEFAULT, i, desc);
	if (mine_ret != ref_ret)
		printf("    " RED "    return value: expected %d, got %d\n" DEFAULT,
			ref_ret, mine_ret);
	if (strcmp(mine_out, ref_out) != 0)
	{
		printf("    " RED "    output: expected \"");
		pf_print_str(ref_out);
		printf("\"\n" DEFAULT);
		printf("    " RED "    output: got      \"");
		pf_print_str(mine_out);
		printf("\"\n" DEFAULT);
	}
	return (-1);
}

#endif
