#!/bin/bash

# Checks that get_next_line is declared in get_next_line_bonus.h with the exact
# prototype the subject defines. Run by test.sh with the mini-moul
# directory as cwd; the student's project is at ../

source ./config.sh
source ./utils/proto_check.sh

check_prototypes "get_next_line_bonus.h" \
	"get_next_line|char *(*p)(int)|char *get_next_line(int fd)"
exit $?
