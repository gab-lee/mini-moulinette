#!/bin/bash

# Exercise 0: ft_hello_garden prints the welcome message.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/growing_code/gc_harness.sh

file=../ex0/ft_hello_garden.py
gc_check_file "$file" print ft_hello_garden
gc_case "ft_hello_garden()" "$file" ft_hello_garden "" "Hello, Garden Community!\n"
exit $pc_error
