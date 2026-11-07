#!/bin/bash

# Exercise 5: ft_water_reminder asks for the days since the last watering (more than 2: water).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/growing_code/gc_harness.sh

file=../ex5/ft_water_reminder.py
gc_check_file "$file" input,int,print ft_water_reminder
for days in 4 1 2 3 0 10; do
	if [ "$days" -gt 2 ]; then
		text="Water the plants!"
	else
		text="Plants are fine"
	fi
	gc_case "$days days" "$file" ft_water_reminder "$days\n" \
		"Days since last watering: $text\n"
done
exit $pc_error
