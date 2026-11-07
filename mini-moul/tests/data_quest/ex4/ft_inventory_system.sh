#!/bin/bash

# Exercise 4: ft_inventory_system parses item:quantity arguments into a dictionary.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/data_quest/pq_harness.sh

file=../ex4/ft_inventory_system.py
pq_check_file "$file" sum,list,round,len,print sys
pq_run "$file" "" "the subject's arguments" sword:1 potion:5 shield:2 armor:3 helmet:1 sword:2 hello key:value
pq_has "redundant, invalid and bad quantity arguments are reported" '[Rr]edundant' "'sword'" "'hello'" "'key'"
pq_has "inventory" "\{'sword': 1, 'potion': 5, 'shield': 2, 'armor': 3, 'helmet': 1\}" "\['sword', 'potion', 'shield', 'armor', 'helmet'\]" '(^|[^0-9])12([^0-9]|$)'
pq_has "percentages" '8\.3%' '41\.7%' '16\.7%' '25\.0%'
pq_has "most and least abundant" '[Mm]ost.*potion.*5' '[Ll]east.*sword.*1' "'magic_item': 1"
pq_compare "the subject's arguments" "=== Inventory System Analysis ===\nRedundant item 'sword' - discarding\nError - invalid parameter 'hello'\nQuantity error for 'key': invalid literal for int() with base 10: 'value'\nGot inventory: {'sword': 1, 'potion': 5, 'shield': 2, 'armor': 3, 'helmet': 1}\nItem list: ['sword', 'potion', 'shield', 'armor', 'helmet']\nTotal quantity of the 5 items: 12\nItem sword represents 8.3%%\nItem potion represents 41.7%%\nItem shield represents 16.7%%\nItem armor represents 25.0%%\nItem helmet represents 8.3%%\nItem most abundant: potion with quantity 5\nItem least abundant: sword with quantity 1\nUpdated inventory: {'sword': 1, 'potion': 5, 'shield': 2, 'armor': 3, 'helmet': 1, 'magic_item': 1}\n"
pq_run "$file" "" "no argument (an empty inventory)"
pq_run "$file" "" "a tie" a:1 b:1
pq_has "a tie: the first item of the command line wins" '[Mm]ost.*(^|[^a-z])a([^a-z]|$)' '[Ll]east.*(^|[^a-z])a([^a-z]|$)'
pq_run "$file" "" "only invalid arguments" nothing x:y
pq_clean
exit $pc_error
