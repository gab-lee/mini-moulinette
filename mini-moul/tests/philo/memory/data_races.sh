#!/bin/bash

# Runs philo built with ThreadSanitizer: a data race (a variable read and
# written by two threads without a mutex) is a failure. The timings are not
# judged here, since the sanitizer slows the program down.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../ (or ../philo)

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo
ph_sanitized tsan || exit 0

for entry in "5 800 200 200 3" "4 310 200 100" "1 800 200 200" "3 800 200 200 2"; do
	# shellcheck disable=SC2206
	args=($entry)
	perl tests/philo/philo_limited.pl 30 .ph_out .ph_err .ph_stat "$PH_SAN_BIN" "${args[@]}"
	read -r ph_code ph_ms ph_leftover ph_sig ph_to < .ph_stat
	if grep -q 'WARNING: ThreadSanitizer' .ph_err; then
		pc_bad "./philo $entry: data race"
		grep -E '^SUMMARY' .ph_err | sort -u | head -3 | while IFS= read -r line; do pc_detail "$line"; done
	elif [ "$ph_to" -eq 1 ]; then
		pc_bad "./philo $entry: still running after 30s"
	elif [ "$ph_sig" -ne 0 ]; then
		pc_bad "./philo $entry: crashed (signal $ph_sig)"
	else
		pc_ok "./philo $entry: no data race"
	fi
done
exit $pc_error
