#!/bin/bash

# Checks the files the subject asks for: a Makefile, headers and sources.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

sources="$(find .. -name '*.c' -not -path '../mini-moul/*' -not -path '../.git/*' | wc -l | tr -d ' ')"
headers="$(find .. -name '*.h' -not -path '../mini-moul/*' -not -path '../.git/*' | wc -l | tr -d ' ')"

if [ -f ../Makefile ]; then
	pc_ok "Makefile exists"
else
	pc_bad "Makefile is missing at the root of your project"
fi
if [ "$sources" -gt 0 ]; then
	pc_ok "$sources .c file(s) found"
else
	pc_bad "no .c file found in your project"
fi
if [ "$headers" -gt 0 ]; then
	pc_ok "$headers .h file(s) found"
else
	pc_bad "no .h file found in your project"
fi
exit $pc_error
