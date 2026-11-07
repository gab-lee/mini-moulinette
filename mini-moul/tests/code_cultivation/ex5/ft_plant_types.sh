#!/bin/bash

# Exercise 5: Flower, Tree and Vegetable inherit from Plant and use super().
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/code_cultivation/cc_harness.sh

file=../ex5/ft_plant_types.py
cc_check_file "$file" print,range,round,super --main-guard --class Plant --subclass Flower:Plant --subclass Tree:Plant --subclass Vegetable:Plant --method Flower.bloom --method Tree.produce_shade --method Flower.show --method Tree.show --method Vegetable.show --uses super
cc_dynamic "$file" ex5
cc_run "$file" ./tests/code_cultivation/expected_ex5.txt
exit $pc_error
