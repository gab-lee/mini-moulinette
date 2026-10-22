#!/bin/bash

# Shared by the philo check scripts. Sourced with the mini-moul directory as
# cwd; the student's project is at ../ (run from the philo folder) or in
# ../philo and ../philo_bonus (run from the folder that holds both).
#
# Timing checks run the program as the Makefile built it. Memory and data
# race checks run copies built with AddressSanitizer / ThreadSanitizer
# (CC is overridden on the make command line); a Makefile that hardcodes
# its compiler makes those checks print a [!] and skip.

source ./utils/program_check.sh

# Seconds a "nobody dies" scenario keeps running, and the longest a scenario
# that must stop by itself may take
PH_ALIVE=${PH_ALIVE:-8}
PH_LIMIT=${PH_LIMIT:-30}

PH_DIR=""
PH_BONUS_DIR=""
ph_code=0
ph_ms=0
ph_leftover=0
ph_info=""

# Where the mandatory and the bonus project are
ph_locate()
{
	if [ -f ../philo/Makefile ]; then
		PH_DIR=../philo
		PH_BONUS_DIR=../philo_bonus
	elif [ -f ../Makefile ]; then
		PH_DIR=..
		PH_BONUS_DIR=../../philo_bonus
	else
		printf "    ${RED}No Makefile found in ../ or ../philo: run mini from your philo folder${DEFAULT}\n"
		exit 1
	fi
}

# ph_prepare <philo|philo_bonus>
# Sets PH_PROJECT (directory) and PH_BIN (the binary), building it if needed.
ph_prepare()
{
	PH_NAME=$1
	ph_locate
	if [ "$PH_NAME" = "philo_bonus" ]; then
		PH_PROJECT=$PH_BONUS_DIR
		if [ ! -f "$PH_PROJECT/Makefile" ]; then
			printf "    ${RED}philo_bonus/Makefile not found next to your philo folder${DEFAULT}\n"
			exit 1
		fi
	else
		PH_PROJECT=$PH_DIR
	fi
	pc_ensure_built "$PH_PROJECT" "$PH_NAME" || exit 1
	PH_BIN="$PH_PROJECT/$PH_NAME"
}

# ph_sanitized <asan|tsan>: PH_SAN_BIN is a sanitizer build of the program.
# Returns 1 (after printing a [!]) when there is none, so the caller can pass.
ph_sanitized()
{
	local kind=$1 flags marker
	if [ "$kind" = "tsan" ]; then
		flags="-fsanitize=thread -g"
		marker=__tsan_init
		if ! ph_tsan_works; then
			pc_warn "ThreadSanitizer cannot run on this machine; data races are not checked"
			return 1
		fi
	else
		flags="-fsanitize=address -g"
		marker=__asan_init
	fi
	if pc_instrumented_build "${kind}_$PH_NAME" "$PH_PROJECT" "$flags" "$marker" "$PH_NAME" all; then
		PH_SAN_BIN=".instr_${kind}_$PH_NAME/$(basename "$(cd "$PH_PROJECT" && pwd)")/$PH_NAME"
		return 0
	fi
	pc_warn "could not build $PH_NAME with the sanitizer (does your Makefile hardcode its compiler?); this check is skipped"
	return 1
}

ph_tsan_works()
{
	local dir
	dir="$(mktemp -d)"
	printf 'int main(void)\n{\n\treturn (0);\n}\n' > "$dir/p.c"
	cc -fsanitize=thread -o "$dir/p" "$dir/p.c" > /dev/null 2>&1 && "$dir/p" > /dev/null 2>&1
	local status=$?
	rm -rf "$dir"
	return $status
}

# ph_run <seconds> <binary> <args...>
# Output in .ph_out / .ph_err; ph_to is 1 when the time ran out, ph_sig is the
# signal that killed the program (0 if none).
ph_run()
{
	local seconds=$1
	shift
	perl tests/philo/philo_limited.pl "$seconds" .ph_out .ph_err .ph_stat "$@"
	read -r ph_code ph_ms ph_leftover ph_sig ph_to < .ph_stat
	if pc_sanitizer_report .ph_err; then
		cat .ph_err >&2
		exit 1
	fi
}

# ph_log <expect> <neighbors 0|1> <partial 0|1> <n> <ttd> <tte> <tts>
# Checks .ph_out; fills ph_info and returns 1 after printing each problem.
ph_log()
{
	local expect=$1 neighbors=$2 partial=$3 n=$4 ttd=$5 tte=$6 tts=$7 report
	local flags=""
	[ "$neighbors" -eq 1 ] && flags="--neighbors"
	[ "$partial" -eq 1 ] && flags="$flags --partial"
	# shellcheck disable=SC2086
	report="$(perl tests/philo/philo_log.pl --n "$n" --ttd "$ttd" --tte "$tte" --tts "$tts" \
		--expect "$expect" $flags < .ph_out)"
	ph_info="$(printf '%s\n' "$report" | sed -n 's/^INFO //p')"
	ph_problems="$(printf '%s\n' "$report" | sed -n 's/^PROBLEM //p')"
	[ -z "$ph_problems" ]
}

# ph_scenario <label> <binary> <alive|die|count:M> <seconds> <neighbors 0|1> <n> <ttd> <tte> <tts> [meals]
#   alive  nobody dies: the program must still be running after <seconds>
#   die    a philosopher dies, and the program stops by itself in time
#   count:M everyone eats M times and the program stops by itself in time
ph_scenario()
{
	local label=$1 bin=$2 expect=$3 seconds=$4 neighbors=$5
	shift 5
	local partial=0 args=("$@")
	[ "$expect" = "alive" ] && partial=1
	ph_run "$seconds" "$bin" "${args[@]}"
	if [ "$expect" = "alive" ]; then
		if [ "$ph_to" -ne 1 ]; then
			pc_bad "$label: the program stopped after $ph_ms ms, but nobody should die (it must keep running)"
			ph_log "$expect" "$neighbors" 0 "$1" "$2" "$3" "$4"
			[ -n "$ph_problems" ] && printf '%s\n' "$ph_problems" | head -3 | while IFS= read -r line; do pc_detail "$line"; done
			[ -s .ph_out ] && pc_detail "last line: $(tail -1 .ph_out)"
			return
		fi
	elif [ "$ph_to" -eq 1 ]; then
		pc_bad "$label: the program was still running after ${seconds}s; it must stop by itself"
		ph_log "$expect" "$neighbors" 1 "$1" "$2" "$3" "$4"
		[ -n "$ph_problems" ] && printf '%s\n' "$ph_problems" | head -3 | while IFS= read -r line; do pc_detail "$line"; done
		return
	elif [ "$ph_sig" -ne 0 ]; then
		pc_bad "$label: the program crashed (signal $ph_sig)"
		return
	fi
	if ph_log "$expect" "$neighbors" "$partial" "$1" "$2" "$3" "$4"; then
		pc_ok "$label: $ph_info"
	else
		pc_bad "$label:"
		printf '%s\n' "$ph_problems" | while IFS= read -r line; do pc_detail "$line"; done
	fi
}
