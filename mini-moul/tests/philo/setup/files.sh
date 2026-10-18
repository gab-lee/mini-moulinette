#!/bin/bash

# Checks the files the subject asks for in the philo folder: a Makefile,
# headers and sources.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_locate
sources="$(find "$PH_DIR" -name '*.c' -not -path '*/mini-moul/*' | wc -l | tr -d ' ')"
headers="$(find "$PH_DIR" -name '*.h' -not -path '*/mini-moul/*' | wc -l | tr -d ' ')"
pc_ok "Makefile exists in $(basename "$(cd "$PH_DIR" && pwd)")"
if [ "$sources" -gt 0 ]; then
	pc_ok "$sources .c file(s) found"
else
	pc_bad "no .c file found in your philo folder"
fi
if [ "$headers" -gt 0 ]; then
	pc_ok "$headers .h file(s) found"
else
	pc_bad "no .h file found in your philo folder"
fi
exit $pc_error
