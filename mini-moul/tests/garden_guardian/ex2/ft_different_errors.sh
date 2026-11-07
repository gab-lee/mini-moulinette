#!/bin/bash

# Exercise 2: four kinds of errors, caught together with one except clause.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/garden_guardian/gg_harness.sh

file=../ex2/ft_different_errors.py
gg_check_file "$file" print,open,int warn --def garden_operations --def test_error_types --try --multi-except
gg_dynamic "$file" ex2
gg_run "$file" ./tests/garden_guardian/expected_ex2.txt
exit $pc_error
