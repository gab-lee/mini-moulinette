#!/bin/bash

# The files the subject asks for, one per exercise directory:
# ex0/ft_garden_intro.py ... ex6/ft_garden_analytics.py. Run from the
# folder that holds ex0 ... ex6 (mini -code_cultivation, or a folder named
# P01).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

for entry in ex0/ft_garden_intro ex1/ft_garden_data ex2/ft_plant_growth \
	ex3/ft_plant_factory ex4/ft_garden_security ex5/ft_plant_types \
	ex6/ft_garden_analytics; do
	if [ -f "../$entry.py" ]; then
		pc_ok "$entry.py"
	else
		pc_bad "$entry.py is missing"
	fi
done
exit $pc_error
