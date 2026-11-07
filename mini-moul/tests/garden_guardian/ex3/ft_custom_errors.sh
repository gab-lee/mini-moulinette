#!/bin/bash

# Exercise 3: GardenError, PlantError and WaterError with default messages.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/garden_guardian/gg_harness.sh

file=../ex3/ft_custom_errors.py
gg_check_file "$file" print check --class GardenError --class PlantError --class WaterError --subclass PlantError:GardenError --subclass WaterError:GardenError --subclass GardenError:Exception --try --raise
gg_dynamic "$file" ex3
gg_run "$file" ./tests/garden_guardian/expected_ex3.txt
exit $pc_error
