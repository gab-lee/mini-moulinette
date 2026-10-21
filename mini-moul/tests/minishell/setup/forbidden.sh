#!/bin/bash

# The subject lists the external functions minishell may use (readline and
# its helpers, the usual system calls, the termcap functions) and allows
# one global variable, for the received signal. The readline variables
# (rl_done, rl_catch_signals, ...) are not counted as functions.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

pc_ensure_built .. minishell || exit 1
pc_check_forbidden .. minishell 1 \
	"readline rl_clear_history rl_on_new_line rl_replace_line rl_redisplay add_history printf malloc free write access open read close fork wait waitpid wait3 wait4 signal sigaction sigemptyset sigaddset kill exit getcwd chdir stat lstat fstat unlink execve dup dup2 pipe opendir readdir closedir strerror perror isatty ttyname ttyslot ioctl getenv tcsetattr tcgetattr tgetent tgetflag tgetnum tgetstr tgoto tputs rl_catch_signals rl_done rl_event_hook rl_instream rl_outstream rl_line_buffer rl_point rl_end" \
	"memset memcpy memmove bzero puts putchar"
exit $pc_error
