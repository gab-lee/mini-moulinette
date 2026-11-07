#!/bin/bash

# Exercise 3: ft_achievement_tracker builds random sets and applies set operations.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_quest/pq_harness.sh

file=../ex3/ft_achievement_tracker.py
pq_check_file "$file" len,print,set random
pq_dynamic "$file" ex3
for run in 1 2 3; do
	pq_run "$file" "" "run $run"
	pq_validate achievements "run $run"
done
pq_clean
exit $pc_error
