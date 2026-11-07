#!/bin/bash

# Exercise 1: ft_garden_name asks for a name, then shows it with the fixed status.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/growing_code/gc_harness.sh

file=../ex1/ft_garden_name.py
gc_check_file "$file" input,print ft_garden_name
for name in "Community Garden" "Rose Corner" "x" "Jardin d'Eden" "42 Beds"; do
	gc_case "name \"$name\"" "$file" ft_garden_name "$name\n" \
		"Enter garden name: Garden: $name\nStatus: Growing well!\n"
done
exit $pc_error
