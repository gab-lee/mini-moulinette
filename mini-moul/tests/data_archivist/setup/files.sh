#!/bin/bash

# The files the subject asks for, one per exercise directory:
# ex0/ft_ancient_text.py ... ex3/ft_vault_security.py. Run from the folder
# that holds ex0 ... ex3 (mini -data_archivist, or a folder named P04).
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/program_check.sh

for entry in ex0/ft_ancient_text ex1/ft_archive_creation \
	ex2/ft_stream_management ex3/ft_vault_security; do
	if [ -f "../$entry.py" ]; then
		pc_ok "$entry.py"
	else
		pc_bad "$entry.py is missing"
	fi
done
exit $pc_error
