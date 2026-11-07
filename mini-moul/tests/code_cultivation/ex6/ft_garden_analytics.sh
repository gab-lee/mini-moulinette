#!/bin/bash

# Exercise 6: static and class methods, Seed(Flower), a nested statistics class, a stats function.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/code_cultivation/cc_harness.sh

file=../ex6/ft_garden_analytics.py
cc_check_file "$file" print,range,round,super,staticmethod,classmethod --main-guard --class Plant --static Plant --classmethod Plant --subclass Seed:Flower --nested Plant --function
cc_dynamic "$file" ex6
cc_run "$file" ./tests/code_cultivation/expected_ex6.txt
exit $pc_error
