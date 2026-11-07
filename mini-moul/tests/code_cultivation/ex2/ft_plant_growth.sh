#!/bin/bash

# Exercise 2: Plant can grow() and age(); a week of growth is simulated.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/code_cultivation/cc_harness.sh

file=../ex2/ft_plant_growth.py
cc_check_file "$file" print,range,round --main-guard --class Plant --method Plant.show --method Plant.grow --method Plant.age
cc_dynamic "$file" ex2
cc_run "$file" ./tests/code_cultivation/expected_ex2.txt
exit $pc_error
