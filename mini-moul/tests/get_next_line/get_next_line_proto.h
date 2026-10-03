#ifndef GET_NEXT_LINE_PROTO_H
# define GET_NEXT_LINE_PROTO_H

/*
** Prototype for get_next_line under test.
**
** The driver only declares the function here; the student's .c files are
** compiled separately by gnl_harness.sh and linked into the driver.
** Nothing from the student's project is ever #include-d.
*/

char	*get_next_line(int fd);

#endif
