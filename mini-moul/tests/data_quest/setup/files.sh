#!/bin/bash

# The files the subject asks for, one per exercise directory:
# ex0/ft_command_quest.py ... ex6/ft_data_alchemist.py. Run from the
# folder that holds ex0 ... ex6 (mini -data_quest, or a folder named P03).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

for entry in ex0/ft_command_quest ex1/ft_score_analytics \
	ex2/ft_coordinate_system ex3/ft_achievement_tracker \
	ex4/ft_inventory_system ex5/ft_data_stream ex6/ft_data_alchemist; do
	if [ -f "../$entry.py" ]; then
		pc_ok "$entry.py"
	else
		pc_bad "$entry.py is missing"
	fi
done
exit $pc_error
