#!/bin/bash

# When a malloc fails, get_next_line must not crash and must not leak: the
# driver makes each malloc call in turn fail (one at a time, for several
# small files) and checks that get_next_line ends with NULL and holds no
# memory. Needs the counters of mini-moulinette's malloc wrapper; without
# them the check is skipped with a [!].
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/get_next_line/gnl_harness.sh

gnl_run mandatory malloc
exit $?
