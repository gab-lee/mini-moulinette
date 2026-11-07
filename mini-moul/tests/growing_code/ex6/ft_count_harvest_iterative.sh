#!/bin/bash

# Exercise 6: ft_count_harvest_iterative counts from 1 to the number asked and ends with "Harvest time!".
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/growing_code/gc_harness.sh

file=../ex6/ft_count_harvest_iterative.py
gc_check_file "$file" input,int,print,range ft_count_harvest_iterative 
for days in 1 5 12 100; do
	expected="Days until harvest: "
	day=1
	while [ $day -le $days ]; do
		expected="${expected}Day $day\n"
		day=$((day + 1))
	done
	gc_case "$days days" "$file" "$(basename "$file" .py)" "$days\n" "${expected}Harvest time!\n"
done
exit $pc_error
