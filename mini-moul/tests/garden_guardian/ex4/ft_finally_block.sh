#!/bin/bash

# Exercise 4: water_plant() raises PlantError, the watering system is closed in a finally block.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/garden_guardian/gg_harness.sh

file=../ex4/ft_finally_block.py
gg_check_file "$file" print check --def water_plant --def test_watering_system --class PlantError --try --finally --raise
gg_dynamic "$file" ex4
gg_run "$file" ./tests/garden_guardian/expected_ex4.txt
exit $pc_error
