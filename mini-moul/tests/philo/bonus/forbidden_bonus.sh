#!/bin/bash

# The bonus allows memset, printf, malloc, free, write, fork, kill, exit,
# pthread_create, pthread_detach, pthread_join, usleep, gettimeofday,
# waitpid and sem_open, sem_close, sem_post, sem_wait, sem_unlink, and
# forbids global variables.
# Run by test.sh with the mini-moul directory as cwd; the student's
# philo_bonus folder is next to the philo folder.

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo_bonus
pc_check_forbidden "$PH_PROJECT" philo_bonus 0 \
	"memset printf malloc free write fork kill exit pthread_create pthread_detach pthread_join usleep gettimeofday waitpid sem_open sem_close sem_post sem_wait sem_unlink" \
	"memcpy memmove bzero puts putchar"
exit $pc_error
