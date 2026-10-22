#!/bin/bash

# The subject asks for the bonus part in separate files with a _bonus
# suffix (*_bonus.c and *_bonus.h).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

sources="$(find .. -name '*_bonus.c' -not -path '../mini-moul/*' -not -path '../.git/*' | wc -l | tr -d ' ')"
headers="$(find .. -name '*_bonus.h' -not -path '../mini-moul/*' -not -path '../.git/*' | wc -l | tr -d ' ')"
if [ "$sources" -gt 0 ]; then
	pc_ok "$sources *_bonus.c file(s) found"
else
	pc_bad "no *_bonus.c file found: the bonus files carry a _bonus suffix"
fi
if [ "$headers" -gt 0 ]; then
	pc_ok "$headers *_bonus.h file(s) found"
else
	pc_bad "no *_bonus.h file found: the bonus files carry a _bonus suffix"
fi
exit $pc_error
