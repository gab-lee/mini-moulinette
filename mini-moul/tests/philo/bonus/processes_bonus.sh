#!/bin/bash

# Each philosopher is a process and the main process is not a philosopher:
# a run with 5 philosophers has at least 5 child processes. When the
# simulation ends, no philosopher process is left running, and (on Linux)
# the named semaphores are removed.
# Run by test.sh with the mini-moul directory as cwd; the student's
# philo_bonus folder is next to the philo folder.

source ./config.sh
source ./tests/philo/philo_harness.sh

ph_prepare philo_bonus

"$PH_BIN" 5 800 200 200 > /dev/null 2>&1 < /dev/null &
parent=$!
sleep 1
children="$(ps -A -o ppid= -o comm= | awk -v p="$parent" '$1 == p && $2 ~ /philo_bonus/' | wc -l | tr -d ' ')"
pkill -KILL -P "$parent" 2> /dev/null
kill -KILL "$parent" 2> /dev/null
wait "$parent" 2> /dev/null
if [ "$children" -ge 5 ]; then
	pc_ok "5 philosophers: $children child processes"
else
	pc_bad "5 philosophers: only $children child process(es) (each philosopher must be a process, the main process is not one)"
fi

sems_before="$(ls /dev/shm/sem.* 2> /dev/null | wc -l | tr -d ' ')"
ph_run 5 "$PH_BIN" 4 310 200 100
if [ "$ph_to" -eq 1 ]; then
	pc_bad "4 310 200 100: still running after 5s"
elif [ "$ph_leftover" -eq 1 ]; then
	pc_bad "4 310 200 100: philosopher processes are still running after the program ended"
else
	pc_ok "4 310 200 100: no process left behind"
fi
sems_after="$(ls /dev/shm/sem.* 2> /dev/null | wc -l | tr -d ' ')"
if [ "$sems_after" -gt "$sems_before" ]; then
	pc_warn "$((sems_after - sems_before)) named semaphore(s) were left in /dev/shm: sem_unlink them so the next run starts clean"
fi
exit $pc_error
