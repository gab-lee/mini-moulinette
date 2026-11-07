#!/bin/bash

# Exercise 7: ft_seed_inventory with type annotations (checked with mypy).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/growing_code/gc_harness.sh

file=../ex7/ft_seed_inventory.py
gc_check_file "$file" print ft_seed_inventory --typed

mypy_cmd="$(gc_tool mypy)"
if [ -z "$mypy_cmd" ]; then
	pc_warn "mypy is not installed, so the type hints are not checked"
elif $mypy_cmd --cache-dir=.mypy_cache "$file" > .gc_mypy 2>&1; then
	pc_ok "mypy reports no error"
else
	pc_bad "mypy reports errors:"
	head -5 .gc_mypy | sed 's/^/        /'
fi
rm -rf .gc_mypy .mypy_cache

gc_case "tomato, 15, packets" "$file" ft_seed_inventory "" \
	"Tomato seeds: 15 packets available\n" '["tomato",15,"packets"]'
gc_case "carrot, 8, grams" "$file" ft_seed_inventory "" \
	"Carrot seeds: 8 grams total\n" '["carrot",8,"grams"]'
gc_case "lettuce, 12, area" "$file" ft_seed_inventory "" \
	"Lettuce seeds: covers 12 square meters\n" '["lettuce",12,"area"]'
gc_case "pepper, 0, packets" "$file" ft_seed_inventory "" \
	"Pepper seeds: 0 packets available\n" '["pepper",0,"packets"]'
gc_case "basil, 1000, grams" "$file" ft_seed_inventory "" \
	"Basil seeds: 1000 grams total\n" '["basil",1000,"grams"]'
gc_case "unknown unit boxes" "$file" ft_seed_inventory "" \
	"Unknown unit type\n" '["tomato",5,"boxes"]'
gc_case "empty unit" "$file" ft_seed_inventory "" \
	"Unknown unit type\n" '["carrot",1,""]'
exit $pc_error
