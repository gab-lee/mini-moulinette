#!/bin/bash

# Runs philo_bonus built with AddressSanitizer: no memory error, no leak,
# and the program still stops by itself.
# Run by test.sh with the mini-moul directory as cwd; the student's
# philo_bonus folder is next to the philo folder.

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo_bonus
ph_sanitized asan || exit 0

for entry in "5 800 200 200 2" "1 200 100 100" "4 310 200 100"; do
	# shellcheck disable=SC2206
	args=($entry)
	ph_run 20 "$PH_SAN_BIN" "${args[@]}"
	if [ "$ph_to" -eq 1 ]; then
		pc_bad "./philo_bonus $entry: still running after 20s"
	elif [ "$ph_sig" -ne 0 ]; then
		pc_bad "./philo_bonus $entry: crashed (signal $ph_sig)"
	else
		pc_ok "./philo_bonus $entry: no memory error, no leak"
	fi
done
exit $pc_error
