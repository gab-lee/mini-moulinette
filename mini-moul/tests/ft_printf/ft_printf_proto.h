#ifndef FT_PRINTF_PROTO_H
# define FT_PRINTF_PROTO_H

/*
** Prototype for ft_printf under test.
**
** The test only declares the function here; the student's .c files are
** compiled separately by test.sh and linked into the test binary.
** Nothing from the student's project is ever #include-d, so their
** headers, helpers and static functions stay out of the test's
** translation unit.
*/

int	ft_printf(const char *format, ...);

#endif
