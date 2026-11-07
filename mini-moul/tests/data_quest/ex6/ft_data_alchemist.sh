#!/bin/bash

# Exercise 6: ft_data_alchemist uses list and dictionary comprehensions.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_quest/pq_harness.sh

file=../ex6/ft_data_alchemist.py
pq_check_file "$file" print,len,sum,round random --count ListComp:2 --count DictComp:2
for run in 1 2 3; do
	pq_run "$file" "" "run $run"
	pq_validate alchemist "run $run"
done
pq_clean
exit $pc_error
