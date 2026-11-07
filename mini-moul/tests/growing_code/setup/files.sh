#!/bin/bash

# The files the subject asks for, one per exercise directory (chapters V to
# XII): ex0/ft_hello_garden.py ... ex7/ft_seed_inventory.py. Run from the
# folder that holds ex0 ... ex7 (mini -growing_code).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

for entry in ex0/ft_hello_garden ex1/ft_garden_name ex2/ft_plot_area \
	ex3/ft_harvest_total ex4/ft_plant_age ex5/ft_water_reminder \
	ex6/ft_count_harvest_iterative ex6/ft_count_harvest_recursive \
	ex7/ft_seed_inventory; do
	if [ -f "../$entry.py" ]; then
		pc_ok "$entry.py"
	else
		pc_bad "$entry.py is missing"
	fi
done
exit $pc_error
