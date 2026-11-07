#!/bin/bash

# Exercise 2: ft_plot_area asks for length and width and shows the area.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/growing_code/gc_harness.sh

file=../ex2/ft_plot_area.py
gc_check_file "$file" input,int,print ft_plot_area
for pair in "5 3 15" "1 1 1" "12 7 84" "100 100 10000" "0 9 0"; do
	set -- $pair
	gc_case "length $1, width $2" "$file" ft_plot_area "$1\n$2\n" \
		"Enter length: Enter width: Plot area: $3\n"
done
exit $pc_error
