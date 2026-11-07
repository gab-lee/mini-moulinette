#!/bin/bash

# Exercise 2: ft_coordinate_system reads 3D coordinates (tuples) and computes distances.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_quest/pq_harness.sh

file=../ex2/ft_coordinate_system.py
pq_check_file "$file" input,round,print math
pq_dynamic "$file" ex2
pq_run "$file" "hello world\n1.0 , 2.5, 3.0\n4,abc,5\n4,5,6\n" "the subject's session"
pq_has "the subject's session" '\(1\.0, 2\.5, 3\.0\)' 'X=1\.0' 'Y=2\.5' 'Z=3\.0' '4\.0311' "abc" '4\.9244'
pq_compare "the subject's session" '=== Game Coordinate System ===\n\nGet a first set of coordinates\nEnter new coordinates as floats in format '"'"'x,y,z'"'"': Invalid syntax\nEnter new coordinates as floats in format '"'"'x,y,z'"'"': Got a first tuple: (1.0, 2.5, 3.0)\nIt includes: X=1.0, Y=2.5, Z=3.0\nDistance to center: 4.0311\n\nGet a second set of coordinates\nEnter new coordinates as floats in format '"'"'x,y,z'"'"': Error on parameter '"'"'abc'"'"': could not convert string to float: '"'"'abc'"'"'\nEnter new coordinates as floats in format '"'"'x,y,z'"'"': Distance between the 2 sets of coordinates: 4.9244\n'
pq_run "$file" "1,2,2\n4,6,2\n" "(1, 2, 2) then (4, 6, 2)"
pq_has "(1, 2, 2) then (4, 6, 2)" '\(1\.0, 2\.0, 2\.0\)' '(^|[^0-9.])3\.0([^0-9]|$)' '(^|[^0-9.])5\.0([^0-9]|$)'
pq_clean
exit $pc_error
