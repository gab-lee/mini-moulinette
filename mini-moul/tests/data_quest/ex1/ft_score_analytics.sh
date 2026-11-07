#!/bin/bash

# Exercise 1: ft_score_analytics computes statistics on the scores given as arguments.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_quest/pq_harness.sh

file=../ex1/ft_score_analytics.py
pq_check_file "$file" len,sum,max,min,print sys
pq_run "$file" "" "five scores" 1500 2300 1800 2100 1950
pq_has "five scores" '\[1500, 2300, 1800, 2100, 1950\]' '(^|[^0-9])5([^0-9]|$)' '9650' '1930\.0' '(^|[^0-9])800([^0-9]|$)' '2300' '1500'
pq_compare "five scores" '=== Player Score Analytics ===\nScores processed: [1500, 2300, 1800, 2100, 1950]\nTotal players: 5\nTotal score: 9650\nAverage score: 1930.0\nHigh score: 2300\nLow score: 1500\nScore range: 800\n'
pq_run "$file" "" "no argument"
pq_has "no argument" '[Uu]sage' 'python3 ft_score_analytics\.py <score1> <score2> \.\.\.'
pq_compare "no argument" '=== Player Score Analytics ===\nNo scores provided. Usage: python3 ft_score_analytics.py <score1> <score2> ...\n'
pq_run "$file" "" "only invalid scores" ab ac
pq_has "only invalid scores" "'ab'" "'ac'" '[Uu]sage'
pq_compare "only invalid scores" "=== Player Score Analytics ===\nInvalid parameter: 'ab'\nInvalid parameter: 'ac'\nNo scores provided. Usage: python3 ft_score_analytics.py <score1> <score2> ...\n"
pq_run "$file" "" "valid and invalid scores" 100 x 300
pq_has "valid and invalid scores: the invalid one is discarded" "'x'" '\[100, 300\]' '(^|[^0-9])400([^0-9]|$)' '200\.0' '(^|[^0-9])200([^0-9]|$)'
pq_run "$file" "" "one score" 42
pq_has "one score" '\[42\]' '42\.0' '(^|[^0-9])0([^0-9]|$)'
pq_clean
exit $pc_error
