#!/bin/bash

# Exercise 1: input_temperature() raises outside 0 to 40 degrees (limits included).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/garden_guardian/gg_harness.sh

file=../ex1/ft_raise_exception.py
gg_check_file "$file" int,print check --def input_temperature --def test_temperature --try --raise
gg_dynamic "$file" ex1
gg_run "$file" ./tests/garden_guardian/expected_ex1.txt
exit $pc_error
