#!/bin/bash

# Exercise 1: a Plant class with a show() method, at least 3 plants.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/code_cultivation/cc_harness.sh

file=../ex1/ft_garden_data.py
cc_check_file "$file" print --main-guard --class Plant --method Plant.show
cc_dynamic "$file" ex1
cc_dynamic "$file" instances 3
cc_run "$file" ./tests/code_cultivation/expected_ex1.txt
exit $pc_error
