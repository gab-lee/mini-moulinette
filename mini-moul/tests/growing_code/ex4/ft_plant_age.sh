#!/bin/bash

# Exercise 4: ft_plant_age says whether a plant is ready (strictly more than 60 days).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/growing_code/gc_harness.sh

file=../ex4/ft_plant_age.py
gc_check_file "$file" input,int,print ft_plant_age
for age in 75 45 60 61 0 365 1; do
	if [ "$age" -gt 60 ]; then
		text="Plant is ready to harvest!"
	else
		text="Plant needs more time to grow."
	fi
	gc_case "age $age" "$file" ft_plant_age "$age\n" \
		"Enter plant age in days: $text\n"
done
exit $pc_error
