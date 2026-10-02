/*
** Linked into every .c test binary by test.sh. Makes stdout unbuffered so
** the case lines printed before an AddressSanitizer abort (which exits
** without flushing stdio) still reach the runner, in order.
*/

#include <stdio.h>

__attribute__((constructor))
static void	unbuffered_stdout(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
}
