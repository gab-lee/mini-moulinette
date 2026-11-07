#!/bin/bash

# Exercise 0: ft_command_quest shows its command-line arguments.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_quest/pq_harness.sh

file=../ex0/ft_command_quest.py
pq_check_file "$file" len,print sys
pq_run "$file" "" "no argument"
pq_has "no argument" 'ft_command_quest\.py' '[Nn]o arguments' '[Tt]otal[^0-9]*1([^0-9]|$)'
pq_compare "no argument" '=== Command Quest ===\nProgram name: ft_command_quest.py\nNo arguments provided!\nTotal arguments: 1\n'
pq_run "$file" "" "hello world 42" hello world 42
pq_has "hello world 42" 'hello' 'world' '42' '[Rr]eceived[^0-9]*3([^0-9]|$)' '[Tt]otal[^0-9]*4([^0-9]|$)'
pq_count "the program name is shown once, not again with the arguments" 'ft_command_quest\.py' 1
pq_compare "hello world 42" '=== Command Quest ===\nProgram name: ft_command_quest.py\nArguments received: 3\nArgument 1: hello\nArgument 2: world\nArgument 3: 42\nTotal arguments: 4\n'
pq_run "$file" "" "one argument with a space" "Data Quest"
pq_has "one argument with a space" 'Argument 1: Data Quest' '[Rr]eceived[^0-9]*1([^0-9]|$)' '[Tt]otal[^0-9]*2([^0-9]|$)'
pq_compare "one argument with a space" '=== Command Quest ===\nProgram name: ft_command_quest.py\nArguments received: 1\nArgument 1: Data Quest\nTotal arguments: 2\n'
pq_clean
exit $pc_error
