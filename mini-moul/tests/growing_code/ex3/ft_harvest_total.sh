#!/bin/bash

# Exercise 3: ft_harvest_total asks for three harvests and shows their total.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/growing_code/gc_harness.sh

file=../ex3/ft_harvest_total.py
gc_check_file "$file" input,int,print ft_harvest_total
for triple in "5 8 3 16" "0 0 0 0" "10 20 30 60" "1 2 3 6" "100 250 7 357"; do
	set -- $triple
	gc_case "harvests $1, $2, $3" "$file" ft_harvest_total "$1\n$2\n$3\n" \
		"Day 1 harvest: Day 2 harvest: Day 3 harvest: Total harvest: $4\n"
done
exit $pc_error
