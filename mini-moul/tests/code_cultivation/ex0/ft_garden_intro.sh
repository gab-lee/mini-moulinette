#!/bin/bash

# Exercise 0: a program that runs directly and shows a plant (no class needed).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/code_cultivation/cc_harness.sh

file=../ex0/ft_garden_intro.py
cc_check_file "$file" print --main-guard

cc_run "$file" ./tests/code_cultivation/expected_ex0.txt
exit $pc_error
