#!/bin/bash

# Exercise 0: input_temperature() converts, test_temperature() catches the error.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/garden_guardian/gg_harness.sh

file=../ex0/ft_first_exception.py
gg_check_file "$file" int,print check --def input_temperature --def test_temperature --try
gg_dynamic "$file" ex0
gg_run "$file" ./tests/garden_guardian/expected_ex0.txt
exit $pc_error
