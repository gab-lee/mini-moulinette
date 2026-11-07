#!/bin/bash

# Exercise 3: plants are created with their information (constructor), at least 5.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/code_cultivation/cc_harness.sh

file=../ex3/ft_plant_factory.py
cc_check_file "$file" print,range,round --main-guard --class Plant --method Plant.__init__ --method Plant.show
cc_dynamic "$file" ex3
cc_dynamic "$file" instances 5
cc_run "$file" ./tests/code_cultivation/expected_ex3.txt
exit $pc_error
