#!/bin/bash

# Runs philo built with AddressSanitizer: no memory error, no leak, and the
# program still stops by itself after a death or the last meal.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo
ph_sanitized asan || exit 0

for entry in "5 800 200 200 2" "2 800 200 200 2" "1 200 100 100" "4 310 200 100"; do
	# shellcheck disable=SC2206
	args=($entry)
	ph_run 20 "$PH_SAN_BIN" "${args[@]}"
	if [ "$ph_to" -eq 1 ]; then
		pc_bad "./philo $entry: still running after 20s"
	elif [ "$ph_sig" -ne 0 ]; then
		pc_bad "./philo $entry: crashed (signal $ph_sig)"
	else
		pc_ok "./philo $entry: no memory error, no leak"
	fi
done
exit $pc_error
