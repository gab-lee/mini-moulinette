#!/bin/bash

# Shared by the get_next_line check scripts. Sourced with the mini-moul
# directory as cwd; the student's project is at ../
#
# get_next_line has no Makefile and must work with any BUFFER_SIZE, so each
# BUFFER_SIZE in GNL_SIZES gets its own build of the student's files linked
# with gnl_driver.c ("default" = compiled without -D BUFFER_SIZE). The
# setup and bonus compile scripts build them; every other script runs them.

GNL_SIZES="default 1 2 5 42 9999 10000000"
GNL_BUILD=".gnl_build"
GNL_DRIVER="tests/get_next_line/gnl_driver.c"
GNL_INSTR="-Dmalloc=gnl_test_malloc -Dfree=gnl_test_free -Dread=gnl_test_read"

gnl_sources()
{
	if [ "$1" = "bonus" ]; then
		printf '%s\n' ../get_next_line_bonus.c ../get_next_line_utils_bonus.c
	else
		printf '%s\n' ../get_next_line.c ../get_next_line_utils.c
	fi
}

gnl_label()
{
	if [ "$1" = "default" ]; then
		printf 'your default BUFFER_SIZE'
	else
		printf 'BUFFER_SIZE=%s' "$1"
	fi
}

# gnl_check_files <file...>
# Each file must exist at the project root; a header must also compile on
# its own and survive double inclusion.
gnl_check_files()
{
	local i=1 error=0 f probe=.gnl_header_probe
	for f in "$@"; do
		if [ -f "../$f" ]; then
			printf "  ${GREEN}${CHECKMARK}${GREY} [$i] %s exists${DEFAULT}\n" "$f"
		else
			printf "    ${RED}[$i] %s is missing at the root of your project${DEFAULT}\n" "$f"
			error=1
		fi
		i=$((i+1))
		case "$f" in
			*.h) [ -f "../$f" ] || continue ;;
			*) continue ;;
		esac
		printf '#include "../%s"\n#include "../%s"\n\nint main(void)\n{\n\treturn (0);\n}\n' \
			"$f" "$f" > "$probe.c"
		if cc -Wall -Wextra -Werror -o "$probe" "$probe.c" 2> "$probe.err"; then
			printf "  ${GREEN}${CHECKMARK}${GREY} [$i] %s compiles on its own and survives double inclusion${DEFAULT}\n" "$f"
		else
			printf "    ${RED}[$i] %s does not compile on its own or breaks when included twice (include guard?):${DEFAULT}\n" "$f"
			sed 's/^/    /' "$probe.err" | head -15
			error=1
		fi
		i=$((i+1))
		rm -f "$probe" "$probe.c" "$probe.err"
	done
	return $error
}

# gnl_build <mandatory|bonus>
# Compiles the student's files once per size in GNL_SIZES and links each
# build with the driver. Objects are built twice: plainly (to report real
# compile errors and to inspect symbols) and with GNL_INSTR, which reroutes
# malloc/free/read to the driver's counters. If only the instrumented build
# fails, the plain one is used and a <binary>.plain marker disables the leak
# and read() checks. Prints one numbered line per size.
gnl_build()
{
	local variant=$1 dir="$GNL_BUILD/$1" i=1 error=0
	local bs flag src name ok instr_ok plain instr

	rm -rf "$dir"
	mkdir -p "$dir"
	if ! cc -Wall -Wextra -Werror -c "$GNL_DRIVER" -o "$dir/driver.o" 2> "$dir/error.txt"; then
		printf "    ${RED}[1] mini-moulinette's own test driver does not compile here:${DEFAULT}\n"
		sed 's/^/    /' "$dir/error.txt" | head -15
		return 1
	fi
	for bs in $GNL_SIZES; do
		flag=""
		[ "$bs" != "default" ] && flag="-D BUFFER_SIZE=$bs"
		ok=1
		instr_ok=1
		plain=()
		instr=()
		for src in $(gnl_sources "$variant"); do
			name="$(basename "${src%.c}")"
			if [ ! -f "$src" ]; then
				printf "%s is missing\n" "$(basename "$src")" > "$dir/error.txt"
				ok=0
				break
			fi
			if ! cc -Wall -Wextra -Werror $flag -c "$src" -o "$dir/$name.$bs.o" 2> "$dir/error.txt"; then
				ok=0
				break
			fi
			plain+=("$dir/$name.$bs.o")
			if cc -Wall -Wextra -Werror $flag $GNL_INSTR -c "$src" -o "$dir/$name.$bs.instr.o" 2> /dev/null; then
				instr+=("$dir/$name.$bs.instr.o")
			else
				instr_ok=0
			fi
		done
		if [ $ok -eq 1 ]; then
			if [ $instr_ok -eq 1 ] && cc -o "$dir/gnl_$bs" "$dir/driver.o" "${instr[@]}" 2> /dev/null; then
				:
			elif cc -o "$dir/gnl_$bs" "$dir/driver.o" "${plain[@]}" 2> "$dir/error.txt"; then
				touch "$dir/gnl_$bs.plain"
			else
				ok=0
			fi
		fi
		if [ $ok -eq 1 ]; then
			if [ "$bs" = "default" ]; then
				printf "  ${GREEN}${CHECKMARK}${GREY} [$i] compiles without -D BUFFER_SIZE${DEFAULT}\n"
			else
				printf "  ${GREEN}${CHECKMARK}${GREY} [$i] compiles with -D BUFFER_SIZE=%s${DEFAULT}\n" "$bs"
			fi
		else
			if [ "$bs" = "default" ]; then
				printf "    ${RED}[$i] does not compile without -D BUFFER_SIZE${DEFAULT}"
			else
				printf "    ${RED}[$i] does not compile with -D BUFFER_SIZE=%s${DEFAULT}" "$bs"
			fi
			if [ -f "$dir/prev_error.txt" ] && cmp -s "$dir/error.txt" "$dir/prev_error.txt"; then
				printf "${RED} (same error as above)${DEFAULT}\n"
			else
				printf "${RED}:${DEFAULT}\n"
				if [ "$bs" = "default" ] && grep -qE "BUFFER_SIZE[^ ]{0,3} undeclared|undeclared identifier [^ ]{0,3}BUFFER_SIZE" "$dir/error.txt"; then
					printf "    ${RED}the subject requires it to compile without the flag too: give BUFFER_SIZE a default in your header (#ifndef BUFFER_SIZE / # define BUFFER_SIZE 42 / #endif)${DEFAULT}\n"
				fi
				sed 's/^/    /' "$dir/error.txt" | head -15
				cp "$dir/error.txt" "$dir/prev_error.txt"
			fi
			error=1
		fi
		i=$((i+1))
	done
	rm -f "$dir/error.txt" "$dir/prev_error.txt"
	return $error
}

# gnl_run <mandatory|bonus> <file|stdin|errors|multi>
# Runs one driver scenario against every build.
gnl_run()
{
	local variant=$1 scenario=$2 error=0 plain_note=0 bs bin mode num
	local where=setup

	[ "$variant" = "bonus" ] && where=compile_bonus
	for bs in $GNL_SIZES; do
		bin="$GNL_BUILD/$variant/gnl_$bs"
		if [ ! -x "$bin" ]; then
			printf "    ${RED}%s: not compiled, see %s${DEFAULT}\n" "$(gnl_label "$bs")" "$where"
			error=1
			continue
		fi
		mode=instr
		if [ -f "$bin.plain" ]; then
			mode=plain
			plain_note=1
		fi
		num=$bs
		[ "$bs" = "default" ] && num=0
		"$bin" "$scenario" "$(gnl_label "$bs")" "$num" "$mode" || error=1
	done
	if [ $plain_note -eq 1 ]; then
		printf "  ${YELLOW}[!] your files could not be built with mini-moulinette's malloc/free/read counters, so leak and read() checks were skipped${DEFAULT}\n"
	fi
	return $error
}

# gnl_check_symbols <mandatory|bonus> [max static variables]
# Inspects the BUFFER_SIZE=42 objects: no global variables, no function
# beyond read/malloc/free and, when given, a cap on static variables.
# Calls the compiler may emit on its own (memset/memcpy for initialising
# arrays, __stack_chk_* for stack protection) are allowed.
gnl_check_symbols()
{
	local variant=$1 max_statics=$2 dir="$GNL_BUILD/$1" error=0
	local objs symbols globals extra statics count

	objs=("$dir"/*.42.o)
	if [ ! -f "${objs[0]}" ]; then
		printf "    ${RED}[1] cannot check: your files do not compile with -D BUFFER_SIZE=42${DEFAULT}\n"
		return 1
	fi
	symbols="$(nm "${objs[@]}" 2> /dev/null | awk 'NF >= 2 { print $(NF-1), $NF }')"
	if [ "$(uname)" = "Darwin" ]; then
		symbols="$(printf '%s\n' "$symbols" | sed 's/ _/ /')"
	fi

	globals="$(printf '%s\n' "$symbols" | awk '$1 ~ /^[BCDGRSV]$/ { print $2 }' | sort -u | paste -sd ' ' -)"
	if [ -z "$globals" ]; then
		printf "  ${GREEN}${CHECKMARK}${GREY} [1] no global variables${DEFAULT}\n"
	else
		printf "    ${RED}[1] global variables are forbidden: %s${DEFAULT}\n" "$globals"
		error=1
	fi

	# Undefined in one file but defined in the other (the student's own
	# helpers) is fine; only calls that leave the student's code count.
	extra="$(printf '%s\n' "$symbols" | awk '
		$1 == "U" { used[$2] = 1; next }
		{ defined[$2] = 1 }
		END { for (s in used) if (!(s in defined)) print s }' | sort \
		| grep -vxE 'read|malloc|free|memset|memcpy|memmove|bzero|__.*|dyld_stub_binder' \
		| paste -sd ' ' -)"
	if [ -z "$extra" ]; then
		printf "  ${GREEN}${CHECKMARK}${GREY} [2] only read, malloc and free are used${DEFAULT}\n"
	else
		printf "    ${RED}[2] uses functions the subject does not allow (only read, malloc and free are; no libft, no lseek): %s${DEFAULT}\n" "$extra"
		error=1
	fi

	if [ -n "$max_statics" ]; then
		statics="$(printf '%s\n' "$symbols" | awk '$1 ~ /^[bd]$/ { print $2 }' | sort -u)"
		count="$(printf '%s' "$statics" | grep -c .)"
		if [ "$count" -le "$max_statics" ]; then
			printf "  ${GREEN}${CHECKMARK}${GREY} [3] uses at most %s static variable${DEFAULT}\n" "$max_statics"
		else
			printf "    ${RED}[3] uses %s static variables (%s); the bonus allows only %s${DEFAULT}\n" \
				"$count" "$(printf '%s\n' "$statics" | paste -sd ' ' -)" "$max_statics"
			error=1
		fi
	fi
	return $error
}
