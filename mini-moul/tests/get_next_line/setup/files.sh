#!/bin/bash

# Checks the mandatory files the subject asks for.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_check_files get_next_line.c get_next_line_utils.c get_next_line.h
exit $?
