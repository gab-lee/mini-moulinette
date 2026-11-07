#!/bin/bash

# The files the subject asks for, one per exercise directory:
# ex0/ft_first_exception.py ... ex4/ft_finally_block.py. Run from the
# folder that holds ex0 ... ex4 (mini -garden_guardian, or a folder named
# P02).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

for entry in ex0/ft_first_exception ex1/ft_raise_exception \
	ex2/ft_different_errors ex3/ft_custom_errors ex4/ft_finally_block; do
	if [ -f "../$entry.py" ]; then
		pc_ok "$entry.py"
	else
		pc_bad "$entry.py is missing"
	fi
done
exit $pc_error
