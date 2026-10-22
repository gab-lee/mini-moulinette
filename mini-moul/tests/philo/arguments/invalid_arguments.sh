#!/bin/bash

# The subject does not say how invalid arguments are reported, only that
# the program takes number_of_philosophers time_to_die time_to_eat
# time_to_sleep [number_of_times_each_philosopher_must_eat]. Here the
# program must stop by itself, without crashing and without starting a
# simulation.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo

# label|arguments (split on spaces)
cases=(
	"no argument|"
	"one argument missing|5 800 200"
	"one argument too many|5 800 200 200 7 9"
	"a letter as number of philosophers|a 800 200 200"
	"letters inside a number|5 8x0 200 200"
	"a letter as time to sleep|5 800 200 abc"
	"a letter as number of meals|5 800 200 200 abc"
	"a negative number of philosophers|-5 800 200 200"
	"a negative time to die|5 -800 200 200"
	"no philosopher|0 800 200 200"
	"a number that does not fit an int|99999999999999999999 800 200 200"
)
for entry in "${cases[@]}"; do
	label="${entry%%|*}"
	# shellcheck disable=SC2206
	args=(${entry#*|})
	ph_run 5 "$PH_BIN" "${args[@]}"
	if [ "$ph_to" -eq 1 ]; then
		pc_bad "$label: still running after 5s; invalid arguments must stop the program"
	elif [ "$ph_sig" -ne 0 ]; then
		pc_bad "$label: crashed (signal $ph_sig)"
	elif grep -qE '^[0-9]+ [0-9]+ (has taken a fork|is eating|is sleeping|is thinking|died)$' .ph_out; then
		pc_bad "$label: the simulation started with invalid arguments"
	else
		pc_ok "$label: stopped without simulating"
	fi
done
exit $pc_error
