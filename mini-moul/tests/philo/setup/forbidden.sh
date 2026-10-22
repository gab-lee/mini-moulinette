#!/bin/bash

# The subject allows memset, printf, malloc, free, write, usleep,
# gettimeofday and the pthread_create, pthread_detach, pthread_join and
# pthread_mutex_* functions, and forbids global variables.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo
pc_check_forbidden "$PH_PROJECT" philo 0 \
	"memset printf malloc free write usleep gettimeofday pthread_create pthread_detach pthread_join pthread_mutex_init pthread_mutex_destroy pthread_mutex_lock pthread_mutex_unlock" \
	"memcpy memmove bzero puts putchar"
exit $pc_error
