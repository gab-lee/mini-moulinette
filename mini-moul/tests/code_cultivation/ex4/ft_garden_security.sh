#!/bin/bash

# Exercise 4: encapsulation: setters and getters, protected attributes, validation.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/code_cultivation/cc_harness.sh

file=../ex4/ft_garden_security.py
cc_check_file "$file" print,range,round --main-guard --class Plant --method Plant.set_height --method Plant.set_age --method Plant.get_height --method Plant.get_age --protected Plant
cc_dynamic "$file" ex4
cc_run "$file" ./tests/code_cultivation/expected_ex4.txt
exit $pc_error
