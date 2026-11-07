#!/bin/bash

# Exercise 5: ft_data_stream uses generators (gen_event, consume_event).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_quest/pq_harness.sh

file=../ex5/ft_data_stream.py
pq_check_file "$file" next,range,len,print random,typing --def gen_event --def consume_event --count Yield:2
pq_dynamic "$file" ex5
for run in 1 2; do
	pq_run "$file" "" "run $run"
	pq_validate stream "run $run"
done
pq_clean
exit $pc_error
